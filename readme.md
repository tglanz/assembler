# Assembler

Assembler for basic assembly language as defined.

## How to compile

```make```

## How to run

Assuming we want to compile the _.as_ files _data/ps.as_ and _data/invalid-strings.as_

```
# From workspace root
bin/assembler data/ps data/invalid-strings
```

## Data

For reference, there are a couple of compiled assembly programs in _data/_ directory

## Logging levels

There are multiple logging levels, each provides different kinds of messages, including lowere levels.

Each level is assigned a value and meaning as below
- 0: Debug - mainly for development
- 1: Info  - general process
- 2: Warning - issues that might need attention
- 3: Error - unrecoverable issues, no output will be generated

To set the level pass -v# to the assembler, it will affect the level of next logs.

Example - will assemble data/ps.as with debug logs
```
# From workspace root
bin/assembler -v0 data/ps
```
