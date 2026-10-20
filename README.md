# SATOS (Spacetime OS) v1.5

A tiny flight-software framework for an **ESP32**, written in Arduino C++. SATOS turns the board into a payload computer that you control with plain text commands over Serial. It tracks altitude milestones, remembers its state across reboots, and runs your own tasks once the payload is deployed.

Licensed under Apache 2.0.

## Features

- **Serial command interface** for flight events and payload control
- **Persistent memory:** altitude, Karman line status, LEO status and payload state survive resets and power loss (stored with the ESP32 `Preferences` library)
- **Watchdog timer** (11 s) that reboots the board if the main loop ever hangs
- **Task block** where you drop in your own code (sensors, cameras, logging) that runs once the payload is deployed
- **Mission log:** milestones and task output are collected in a string, ready to be sent to "earth"

## Requirements

- An ESP32 board
- [PlatformIO](https://platformio.org/) (or the Arduino IDE with ESP32 support)
- A Serial monitor at **115200 baud**

## Getting started

1. Clone this repo and open it in PlatformIO.
2. Build and upload to your ESP32.
3. Open the Serial monitor at 115200 baud.
4. Wait about 6 seconds for boot, then type `help` followed by a capital `X` and send it.

## Commands

**Every command ends with a capital `X`.** For example: `deployX`

Never use a capital `X` inside a command name, because `X` is what tells SATOS the command is finished.

| Command | What it does |
|---|---|
| `alt100m` | Set altitude to 100 m |
| `alt500m` | Set altitude to 500 m |
| `alt800m` | Set altitude to 800 m |
| `alt1.5km` | Set altitude to 1500 m |
| `altkarmapass` | Pass the Karman line (100 km) |
| `leo` | Mark low earth orbit as reached |
| `deploy` | Start payload tasks (send again to stop) |
| `help` | Show the command list |

Example session:

```
> alt500mX
NOW IS TRUE = e500m
> deployX
Deploying payload
```

## Adding your own code

Look for the comments in `main.cpp`:

- **Custom functions and variables:** declare them near the top, under `DECLARE YOUR CUSTOM FUNCTIONS HERE`. `Gettemp()` is an example that reads the chip temperature.
- **New commands:** add another `else if (input == "yourcommand")` branch where it says `ADD YOUR COMMANDS HERE`, and add a line for it in the `help` text.
- **Payload tasks:** put code that should run in space inside the `if (starttask)` block. Add anything you want sent to earth to `retStr`.

## How it works

On boot, SATOS restores its saved state, starts the watchdog, and enters the main loop. Each loop it feeds the watchdog, reads the CPU temperature, checks for a command, updates altitude flags, and runs the payload tasks if they are deployed. Passing 100 km triggers the Karman line event once and saves it, so it will not repeat after a reboot.

## Roadmap

- Sensor and camera modules
- A ground automation script that sends commands from "earth"
- Low power mode

## Contributing

Issues and pull requests are welcome. If you add a module, keep it in its own clearly marked section so it is easy to find.

## License

Apache License 2.0. See [LICENSE](LICENSE).
