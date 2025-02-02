# spinny

To run locally or develop, you'll need [nix](https://nixos.org/download/) the package manager.

### Running locally

1. Go into the developer shell using `nix develop` at the project root
2. Build using `nix build`
2. Run `emrun result/main.html`

### Developing
To run incremental builds of the project, do the following:
1. Go into the developer shell using `nix develop` at the project root
2. Make a directory called `build` at the project root and `cd` into it
3. Run `emcmake cmake ..`
4. Run `emmake make`
  - Alternatively, if you also want to copy the configuration and compilation database for clangd at the project root, run `emmake make develop`
5. The output will be in `build/out`
