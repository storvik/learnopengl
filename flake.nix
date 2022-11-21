{
  description = "C++ library description";

  inputs = {

    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

    flake-utils = {
      url = "github:numtide/flake-utils";
      inputs.nixpkgs.follows = "nixpkgs";
    };

    gitignore = {
      url = "github:hercules-ci/gitignore.nix";
      flake = false;
    };

    nixGL = {
      url = "github:guibou/nixGL";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.flake-utils.follows = "flake-utils";
    };

  };

  outputs = { self, nixpkgs, gitignore, flake-utils, nixGL }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [
            nixGL.overlay
          ];
        };
        gitignoreSource = (import gitignore { inherit (pkgs) lib; }).gitignoreSource;
      in
      rec {
        packages.learnopengl = with pkgs; stdenv.mkDerivation {
          name = "learnopengl";
          version = "0.0.1";

          src = gitignoreSource ./.;

          nativeBuildInputs = [
            cmake
            ninja
            gdb
            gtest
          ];

          buildInputs = [
            spdlog
            mesa
            libcxx
            libGL
            SDL2
            SDL2_image
            glew
            glm
            glfw
            freetype
            freeglut
          ];
        };

        devShell = pkgs.mkShell {
          buildInputs = [
            pkgs.nixgl.auto.nixGLDefault
            packages.learnopengl.nativeBuildInputs
            packages.learnopengl.buildInputs
          ];
        };

        defaultPackage = packages.learnopengl;

      });
}
