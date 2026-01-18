# webserver

A lightweight web server that serves local files.

No dependencies and (as of now) no HTTPS support.

## How to use

Create the directory `$HOME/.config/dioserver`, and add route information to it.
information. The format is as follows:

```text
<route1> <file1>
<route2> <file2>
...
```

You can have up to `MAXROUTES` routes (set at compile time in `config.h`).

## Build

Create the `bin` and `obj` directories. Customize the `config.h` file. Then run

```shell
make
```

## Notes

Expected features:
- Keep-alive
- HTTPS
