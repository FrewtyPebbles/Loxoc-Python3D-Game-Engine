from __future__ import annotations
from os import getenv, listdir, path
import sys, os
from typing import Generic, TypeVar
from setuptools import find_packages, setup, Extension
from Cython.Build import cythonize
import pkgconfig as pcfg
from setuptools.command.build_ext import build_ext
from setuptools.command.install import install as st_install
from setuptools.command.sdist import sdist
from setuptools import Command
from wheel.bdist_wheel import bdist_wheel

MODULE_NAME = "Runespoor"

DEV_VERSION = 14

VERSION = f"1.0.0.dev{DEV_VERSION}"

print(f"BUILDING {MODULE_NAME}-V{VERSION}\n"
    "Note: If building from source, ensure that GLM is installed inside whatever package manager you are using's include directory, make sure pkg-config is installed and include the flag `--RUNEcompiler=<clang|gcc|msvc>` or `--config-settings=\"--RUNEcompiler=<clang|gcc|msvc>\"` with your default compiler.  Otherwise the build will fail."
"")

C_PATH = "src"

INCLUDE_DIRS = [
    "glad/include/",
    "stb/",
    *[inc.removeprefix("-I") for inc in pcfg.cflags("sdl2").split()],
    *[inc.removeprefix("-I") for inc in pcfg.cflags("assimp").split()]
]
LIBRARY_DIRS = [
    *[inc.removeprefix("-L") for inc in pcfg.libs("sdl2", True).split() if inc.startswith("-L")],
    *[inc.removeprefix("-L") for inc in pcfg.libs("assimp", True).split() if inc.startswith("-L")]
]

LIBRARIES = [
    *[inc.removeprefix("-l") for inc in pcfg.libs("sdl2", True).split() if not inc.startswith("-L")],
    *[inc.removeprefix("-l") for inc in pcfg.libs("assimp", True).split() if not inc.startswith("-L")]
]

c_deps = [
    *[path.join(C_PATH, cppf) for cppf in listdir(C_PATH) if cppf.endswith(".cpp")],
    "glad/src/gl.c"
]

readme_src = (fp:=open(path.join(path.dirname(__file__), "readme.md"), "r")).read()
fp.close()

included_pkg_data=[
    'LICENSE',
    '*.glsl',
    'requirements.txt',
    'readme.md',
]

flags = {
    "clang":[
        "-static-libgcc -static-libstdc++",
        "-std=c++20",
        "-O3"
    ],
    "gcc":[
        "-static-libgcc -static-libstdc++",
        "-std=c++20",
        "-O3"
    ],
    "msvc":[
        "/MT",
        "/std:c++20",
        "/MP",
        "/Ox"
    ],
}

default_compiler = "clang"

def command_factory(command:type[Command]) -> type:
    class st_CLIARGS(command):
        user_options = [
            ('RUNEcompiler=', None, "Set this to your default compiler if building from source.")
        ]

        def initialize_options(self):
            self.RUNEcompiler = default_compiler
            super().initialize_options()

        def set_undefined_options(self, src_cmd: str, *option_pairs: tuple[str, str]) -> None:
            self.RUNEcompiler = default_compiler
            super().set_undefined_options(src_cmd, *option_pairs)

        def finalize_options(self):
            assert self.RUNEcompiler in flags.keys(), f"'{self.RUNEcompiler}' is not a valid compiler option for `--RUNEcompiler=<clang|gcc|msvc>`"
            super().finalize_options()
    
    return st_CLIARGS

class build_ext_custom(command_factory(build_ext)):
    def run(self):
        if self.RUNEcompiler:
            ext:Extension = self.extensions[0]
            ext.extra_compile_args = flags[self.RUNEcompiler]
        super().run()


EXTENSIONS = [
    Extension(f"{MODULE_NAME}.core",
            sources=[path.join(MODULE_NAME, "core" + ".pyx"), *c_deps],
            language="c++",
            include_dirs=INCLUDE_DIRS,
            libraries=LIBRARIES,
            library_dirs=LIBRARY_DIRS,
            )
]

setup(
    cmdclass={'build_ext':build_ext_custom, 'install':command_factory(st_install), 'sdist':command_factory(sdist), 'bdist_wheel':command_factory(bdist_wheel)},
    name=MODULE_NAME,
    version=VERSION,
    author="William Lim",
    description = ""
    "Runespoor is a flexible, straight forward, multi-paradigm game engine that is built from the ground up with developer experience in mind."
    "Utilizing a game loop system similar to pygame, you are able to abstract elements of your game away from the game loop as much as you'd like."
    "",
    long_description=readme_src,
    long_description_content_type="text/markdown",
    license="MIT",
    keywords="game engine sdl opengl 3d quaternion glm vec3 vector3",
    url="https://github.com/FrewtyPebbles/Runespoor-Python3D-Game-Engine",
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "License :: OSI Approved :: MIT License",
        "Intended Audience :: Developers",
        "Topic :: Games/Entertainment"
    ],
    ext_modules = cythonize(EXTENSIONS,
        compiler_directives={
            "language_level": 3,
            "profile": False
        },
    ),
    packages=find_packages(exclude=[
        "./Runespoor/core.cp311-win_amd64.pyd",
        "./Runespoor/core.cpp"
        "./test.py"
    ]),
    include_package_data=True,
    package_data={
        MODULE_NAME: included_pkg_data
    },
)