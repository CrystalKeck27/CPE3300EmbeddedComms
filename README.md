# probe-rs-c-template

Template project for a c project with probe-rs as a debugger.


## Dependencies

- arm-none-eabi toolkit
- probe-rs

as of 2024-02-24, the c project tooling is still under development. The most up-to-date git version of probe-rs should be used until further notice.

### Arch Linux
```sh
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib
cargo install --git https://github.com/probe-rs/probe-rs probe-rs --features=cli
```
If cargo install fails because probe-rs is already installed, run with `--force`.

### Windows
```pwsh
winget install Arm.GnuArmEmbeddedToolchain
irm https://github.com/probe-rs/probe-rs/releases/latest/download/probe-rs-tools-installer.ps1 | iex
```
