from collections import defaultdict
from os import listdir, path
import os
from setuptools import find_packages, setup, Extension
from Cython.Build import cythonize
import pkgconfig as pcfg
from setuptools.command.build_ext import build_ext

MODULE_NAME = "Loxoc"

DEV_VERSION = 21

VERSION = f"1.0.0.dev{DEV_VERSION}"

print(f"BUILDING {MODULE_NAME}-V{VERSION}\n"
    "Note: If building from source, ensure that GLM is installed inside whatever package manager you are using's include directory, make sure pkg-config is installed.  Otherwise the build will fail."
"")

C_PATH = "src"
sdl2 = pcfg.variables("sdl2")
assimp = pcfg.variables("assimp")
INCLUDE_DIRS = [
    "glad/include/",
    "stb/",
    sdl2["includedir"],
    assimp["includedir"]
]
LIBRARY_DIRS = [
    sdl2["libdir"],
    assimp["libdir"]
]

LIBRARIES = [
    *[inc.replace("-l", "") for inc in pcfg.libs("sdl2", True).split() if inc.startswith("-l")],
    *[inc.replace("-l", "") for inc in pcfg.libs("assimp", True).split() if inc.startswith("-l")]
]

c_deps = [
    *[path.join(C_PATH, cppf) for cppf in listdir(C_PATH) if cppf.endswith(".cpp")],
    "glad/src/gl.c"
]
with open(path.join(path.dirname(__file__), "readme.md"), "r") as fp:
    readme_src = fp.read()

included_pkg_data=[
    'LICENSE',
    '*.glsl',
    'requirements.txt',
    'readme.md',
]

EXTENSIONS = [
    Extension(f"{MODULE_NAME}.core",
            sources=[path.join(MODULE_NAME, "core" + ".pyx"), *c_deps],
            language="c++",
            include_dirs=INCLUDE_DIRS,
            libraries=LIBRARIES,
            library_dirs=LIBRARY_DIRS,
    )
]

BUILD_ARGS = defaultdict(lambda: ['-O3', '-g0'])
for compiler, args in [
    ('msvc', [
        "/MT",
        "/std:c++20",
        "/MP",
        "/Ox",
    ]),
    ('gcc', [
        '-O3',
        '-std=c++20',
        '-static',
        '-fpermissive'
    ])
]:
    BUILD_ARGS[compiler] = args
    
class build_ext_compiler_check(build_ext):
    def build_extensions(self):
        compiler = self.compiler.compiler_type
        print(f"COMPILER USED: {compiler}")
        args = []
        if compiler == 'msvc':
            args = BUILD_ARGS['msvc']
        else:
            args = BUILD_ARGS['gcc']
        for ext in self.extensions:
            ext.extra_compile_args = args
        build_ext.build_extensions(self)

setup(
    cmdclass={ 'build_ext': build_ext_compiler_check },
    name=MODULE_NAME,
    version=VERSION,
    author="William Lim",
    description = ""
    "Loxoc is a flexible, straight forward, multi-paradigm game engine that is built from the ground up with developer experience in mind."
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
        "./Loxoc/core.cp311-win_amd64.pyd",
        "./Loxoc/core.cpp"
        "./test.py"
    ]),
    include_package_data=True,
    package_data={
        MODULE_NAME: included_pkg_data
    },
)