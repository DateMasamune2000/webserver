# webserver

A lightweight web server that serves local files.

No dependencies and (as of now) no HTTPS support.

## How to use

### Arguments

One optional argument can be given to specify the current working directory. If
the argument is not present, the working directory is assumed to be the parent
directory of the executable. All relative paths in the configuration file are
expressed relative to the working directory.

### Configuration

The configuration file is located at `$HOME/.config/dioserver`, and contains
route information in the following format:

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
