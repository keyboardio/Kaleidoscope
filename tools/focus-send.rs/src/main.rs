// focus-send -- Bare-bones Focus testing tool
// Copyright (C) 2022  Keyboard.io, Inc.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

use clap::Parser;
use serialport::SerialPort;
use std::io::{self, Write};
use std::thread;
use std::time::Duration;

#[derive(Parser)]
#[clap(version, about)]
struct Cli {
    #[clap(
        short,
        long,
        env,
        hide_env = true,
        value_name = "PATH",
        help = "The device to connect to"
    )]
    device: Option<String>,

    command: String,
    args: Vec<String>,
}

fn main() {
    let opts = Cli::parse();
    let device = match opts.device() {
        None => {
            eprintln!("No device found to connect to");
            ::std::process::exit(1);
        }
        Some(d) => d,
    };

    let mut port = serialport::new(&device, 11520)
        .timeout(Duration::from_millis(100))
        .open()
        .unwrap_or_else(|e| {
            eprintln!("Failed to open \"{}\". Error: {}", &device, e);
            ::std::process::exit(1);
        });

    flush(&mut port);

    send_request(&mut port, opts.command, opts.args)
        .expect("failed to send the request to the keyboard");

    wait_for_data(&*port);

    let reply = read_reply(&mut port).expect("failed to read the reply");
    println!("{}", reply);
}

impl Cli {
    fn device(self: &Cli) -> Option<String> {
        const SUPPORTED: [[u16; 2]; 3] = [
            // Keyboardio Model100
            [0x3496, 0x0006],
            // Keyboardio Atreus
            [0x1209, 0x2303],
            // Keyboardio Model01
            [0x1209, 0x2301],
        ];

        // If we had a device explicitly specified, use that.
        if let Some(device) = &self.device {
            return Some(device.to_string());
        }

        // Otherwise list the serial ports, and return the first USB serial port
        // that has a vid/pid that matches any of the Keyboardio devices.
        let ports_ = serialport::available_ports();
        if ports_.is_err() {
            return None;
        }
        let ports = ports_.unwrap();
        if ports.is_empty() {
            return None;
        }

        for port in ports.iter() {
            if let serialport::SerialPortType::UsbPort(port_info) = &port.port_type {
                for p in SUPPORTED.iter() {
                    let [vid, pid] = p;
                    if port_info.vid == *vid && port_info.pid == *pid {
                        return Some(port.port_name.to_string());
                    }
                }
            }
        }

        // If we found no supported devices, bail out. The user can still
        // specify a device directly, if we fail to autodetect one.
        None
    }
}

// Send an empty command, and consume any replies. This should clear any pending
// commands or output.
fn flush(port: &mut Box<dyn SerialPort>) {
    send_request(port, String::from(" "), vec![]).expect("failed to send an empty command");
    wait_for_data(&**port);
    read_reply(port).expect("failed to flush the device");
}

fn send_request(
    port: &mut Box<dyn SerialPort>,
    command: String,
    args: Vec<String>,
) -> Result<(), std::io::Error> {
    let request = [vec![command], args].concat().join(" ") + "\n";

    port.write_data_terminal_ready(true).unwrap();
    port.write_all(request.as_bytes())
}

fn wait_for_data(port: &dyn SerialPort) {
    while port.bytes_to_read().expect("Error calling bytes_to_read") == 0 {
        thread::sleep(Duration::from_millis(100));
    }
}

fn read_reply(port: &mut Box<dyn SerialPort>) -> Result<String, std::io::Error> {
    let mut buffer: Vec<u8> = vec![0; 1024];
    let mut result: String = String::from("");

    port.read_data_set_ready().unwrap();

    loop {
        match port.read(buffer.as_mut_slice()) {
            Ok(t) => {
                result = result + &String::from_utf8_lossy(&buffer[..t]);
            }
            Err(ref e) if e.kind() == io::ErrorKind::TimedOut => {
                break;
            }
            Err(e) => {
                return Err(e);
            }
        }

        thread::sleep(Duration::from_millis(100));
    }

    Ok(cleanup_reply(result))
}

fn cleanup_reply(reply: String) -> String {
    reply
        .lines()
        .filter(|l| !l.is_empty() && *l != ".")
        .collect::<Vec<&str>>()
        .join("\n")
}

#[cfg(test)]
mod test {
    #[test]
    fn cleanup_reply() {
        assert_eq!(
            super::cleanup_reply(String::from("line1\nline2\r\nline3")),
            "line1\nline2\nline3"
        );
    }
}
