{
  description = "visua-web";

  inputs = {
      nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
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

        # dev environment (clangd)
        clang-tools
        llvm.clang
        nodePackages.typescript-language-server
        nodePackages.vscode-langservers-extracted
        
        # libraries
        glibc_multi
        SDL2

        # python for gdb script
        python3
      ];
      packageParams = {
        nativeBuildInputs = buildInputs;
        src = ./.;
      };
    in
    {
      #defaultPackage.${system} = pkgs.stdenv.mkDerivation {
      #  name = "visua-web";
      #  inherit buildInputs;
      #  src = ./.;
      #  configurePhase = "";
      #  buildPhase = ''
      #    emcc src/main.cpp -o $out/woof.html
      #  '';
      #  installPhase = "";
      #};
      defaultPackage.${system} = pkgs.runCommand "visua-web" packageParams ''
        export HOME=$(mktemp -d)
        mkdir -p $out
        emcc $src/src/main.cpp -o $out/main.html
        cp $src/src/woof.html $src/src/woof.js $out

        emcc -std=c++20 $src/src/hello_triangle_minimal.cpp -sUSE_SDL=2 -sWASM=1 -o $out/hello_triangle_minimal.js
        #cp $src/src/hello_triangle_minimal.html $out
      '';

      devShell.${system} = pkgs.mkShell {
        buildInputs = buildInputs ++ [
          pkgs.pkg-config
        ];
        shellHook = ''
          EMSCRIPTEN_ROOT=${pkgs.emscripten}
        '';
      };
    };
}
