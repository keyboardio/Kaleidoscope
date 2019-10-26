# Usage:
#
# > find-device-port-cygwin.ps1 '1209' '2300' -Format COM
# COM7
# > find-device-port-cygwin.ps1 '1209' '2300' -Format WSL
# /dev/ttyS7
# > find-device-port-cygwin.ps1 '1209' '2300' -Format Cygwin
# /dev/ttyS6
Param(
    [string]$VendorID,
    [string]$ProductID, # Careful; $PID is a different builtin
    [ValidateSet('COM','Cygwin','WSL')][string]$Format
)

$DeviceParametersRegKey = @(Get-ChildItem -ErrorAction SilentlyContinue -Recurse 'HKLM:\SYSTEM\CurrentControlSet\Enum' |
    Where-Object Name -match "VID_$VendorID&PID_$ProductID" |
    Where-Object Property -eq PortName)

if ($DeviceParametersRegKey.Count -eq 0) {
    throw "Could not find any devices matching VID $VendorID and PID $ProductID which were mapped to a COM port"
}

if ($DeviceParametersRegKey.Count -ge 2) {
    throw "More than one devices matching VID $VendorID and PID $ProductID were found mapped to a COM port"
}

# This will be of form 'COM6'
$COMPortName = ($DeviceParametersRegKey | Get-ItemProperty).PortName
$COMPortNumber = [int]$COMPortName.Substring(3)

if ($Format -eq 'COM') {
    $Output = $COMPortName
} elseif ($Format -eq 'WSL') {
    $Output = "/dev/ttyS$COMPortNumber"
} elseif ($Format -eq 'Cygwin') {
    $CygwinPortNumber = $COMPortNumber - 1
    $Output = "/dev/ttyS$CygwinPortNumber"
}

# "-NoNewline" below is important to prevent bash from seeing an extra trailing '\r'
Write-Host -NoNewline $Output
