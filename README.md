# CT File Encrypt 

### Introduction ###
CT File Encrypt can encrypt your private documents using strong AES-256 encryption without password.

Encryption and decryption are easy. Just drop the file into it, that it! No password required. It is designed for encrypting small size (< 10MB) documents, photo, screenshots, etc. before uploading to cloud storage or restrict others to access.

The private key will be generated automatically in %USERPROFILE%\key for the first time. The key is unique, SAVE it to a secure and safe place. You can zip it with password (7z with AES-256 protected recommended) for the backup. DON'T lost it otherwise you won't be able to open your encrypted files anymore.

Note: Folders are not supported yet. But you can drag multiple files into it.

### Dependencies ###
* OpenSSL 1.0.1
* Qt 4.8.6

### Platforms ###
Available for Windows and Mac OS X (coming).

### Know Issues / Future Development ###
* Mac OS X support
* Large files optimization
* Folder support
* Compile in MinGW will lost support for non-ASCII file names

Enjoy! Welcome for forking and pull request.