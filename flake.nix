{
  description = "spinny";

  inputs = {
      nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
      sdl = {
        url = "https://github.com/libsdl-org/SDL/archive/release-2.30.9.zip";
        flake = false;
      };
  };

  outputs = inputs:
    let
      system = "x86_64-linux";
      pkgs = import inputs.nixpkgs {
          inherit system;
      };
      llvm = pkgs.llvmPackages_latest;
      buildInputs = with pkgs; [
        # build tools
        cmake
        gnumake
        makeWrapper
        gdb
        emscripten
        nodejs_22
        curl

        # dev environment (clangd)
        llvm.clang-unwrapped
        nodePackages.typescript-language-server
        nodePackages.vscode-langservers-extracted
        glsl_analyzer
        
        # libraries
        glibc_multi
        SDL2
        glm

        # python for gdb script
        python3
      ];
      packageParams = {
        nativeBuildInputs = buildInputs;
        inherit buildInputs;
        src = ./.;
      };
      setEnvVariables = ''
        export EMSCRIPTEN_ROOT=${pkgs.emscripten}

        export EM_CACHE=/tmp/.spinny-emscripten-cache
        export EMCC_LOCAL_PORTS="sdl2=${inputs.sdl}"

        rm -rf EM_CACHE

        export GLM_PATH=${pkgs.glm}
        export glm_DIR=${pkgs.glm}/share/glm
      '';
    in
    {
      defaultPackage.${system} = pkgs.runCommand "spinny" packageParams ''
        export HOME=$(mktemp -d)
        ${setEnvVariables}

        emcmake cmake $src
        emmake make

        mkdir -p $out
        cp out/* $out
      '';

      devShell.${system} = pkgs.mkShell {
        buildInputs = buildInputs ++ [
          pkgs.pkg-config
        ];
        shellHook = ''
          ${setEnvVariables}
        '';
      };
    };
}
