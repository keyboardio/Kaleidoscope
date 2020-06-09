# Design philosophy

Kaleidoscope should, in order:  

1) Work well as a keyboard 
2) Be compatible with real hardware 
3) Be compatible with the spec 
4) Be easy to read and understand 
5) Be easy to modify and customize

Our code indentation style is managed with 'make astyle.' For code we 'own', there should be an astyle target in the Makefile. For third party code we use and expect to update (ever), we try not to mess with the upstream house style.
