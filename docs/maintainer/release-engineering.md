# Building a numbered release of Kaleidoscope for Arduino

1. Tag the release in the Kaleidoscope repo

```git tag -s v1.99.9```


```git push --tags```


2. Tag the GD32 Arduino core
```
git clone https://github.com/keyboardio/ArduinoCore-GD32-Keyboardio
cd ArduinoCore-GD32-Keyboardio
git tag -s v1.99.9
git push --tags
```

3. Tag the AVR and Virtual Arduino cores

```
git clone https://github.com/keyboardio/Kaleidoscope-Bundle-Keyboardio
cd Kaleidoscope-Bundle-Keyboardio
git tag -s v1.99.9
git push --tags
```


4. Build the package

Back inside a Kaleidoscope master checkout
```perl bin/build-arduino-package --kaleidoscope-tag=v1.99.9 --bundle-tag=v1.99.9 --arch-gd32-tag=v1.99.9```

This will check out fresh copies of all three repositories and their submodules, build arduino board index files, and package everything up. It will then tell you you need to verify the contents of the repo with a message like 

"Now, you need to cd to /var/folders/80/n7lx0k152rd38svj1q8gsqvm0000gn/T/dri7ffIdp8/boards_manager_repo check the content and push it"

Compare the new release to the previous release. At the very least, look over the size of the built packages to make sure it's not unexpectedly larger or smaller and that the updated index files look like they have the correct form and content.

If everything looks good, it's time to push the boards_manager_repo changes to github, which will make the new release available.
