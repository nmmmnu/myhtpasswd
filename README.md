myhtpasswd
==========
---
Utility for change the password of dovecot intallations.

It basically do following:

```
/usr/bin/doveadm auth test user@domain old_pass         && \
/usr/local/bin/htpasswd -sb /VMAIL/AUTH/domain user new_pass
```
It uses **doveadm auth**, instead of **htpasswd -v** because on some dovecot installations, there are password schemas that are not recognized by **htpasswd**.

Because of security, everything is hardcoded. You need to change it to match your configuration.
