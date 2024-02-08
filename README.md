# pam_simple

A simple [PAM](https://en.wikipedia.org/wiki/Linux_PAM) module to authenticate users on Linux-based Operating Systems,
via ``/etc/shadow``.
The module also supports user banning.

**TODO List**
Auth Management Group:
- [ ] implement parser for ``/etc/shadow`` to extract information such as: username, hash algo, salt, etc.
- [ ] implement SHA-512, SHA-256 and MD5 hash functions (preferably use cryptlib) w.r.t salt
- [ ] implement the PAM function for credentials validation (using the above necessities)

Account Management Group:
- [ ] get path to banned users file from PAM config file and check for existence of ``username`` (hence banned)
- [ ] check if password is expired according to ``/etc/shadow``


## Installation
Make sure you have ``libpam-dev``:
```
$ sudo apt install libpam-dev
```


## Docker and Testing
...


## Resources
- [PAM configuration files](https://web.archive.org/web/20190420035810/https://fedetask.com/linx-pam-configuration-tutorial/)
- [Use PAM service in an application](https://web.archive.org/web/20190420073246/https://fedetask.com/writing-a-linux-pam-aware-application/)
- [What can be expected by the application?](https://web.archive.org/web/20190502211945/http://www.linux-pam.org/Linux-PAM-html/adg-interface-by-app-expected.html#adg-pam_get_item)
- [Writing a Linux PAM module](https://web.archive.org/web/20190523222819/https://fedetask.com/write-linux-pam-module/)
- [Understanding /etc/shadow file format on Linux](https://www.cyberciti.biz/faq/understanding-etcshadow-file/)
