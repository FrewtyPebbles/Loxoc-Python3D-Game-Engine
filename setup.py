from os import getenv, listdir, path
import os
from setuptools import find_packages, setup, Extension
from Cython.Build import cythonize
import pkgconfig as pcfg

print(""
    "Note: If building from source, ensure that GLM is installed inside whatever package manager you are using's include directory.  Otherwise the build will fail."
"")

MODULE_NAME = "Runespoor"

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

EXTENSIONS = [
    Extension(f"{MODULE_NAME}.core",
              sources=[path.join(MODULE_NAME, "core" + ".pyx"), *c_deps],
              language="c++",
              include_dirs=INCLUDE_DIRS,
              libraries=LIBRARIES,
              library_dirs=LIBRARY_DIRS,
              extra_compile_args=["/MT", "/std:c++20", "/MP", "/Ox"]
              )
]

readme_src = (fp:=open(path.join(path.dirname(__file__), "readme.md"), "r")).read()
fp.close()

setup(
    name=MODULE_NAME,
    version="1.0.0.dev1",
    author="William Lim",
    description = """
    Runespoor is a flexible, straight forward, multi-paradigm game engine that is built from the ground up with developer experience in mind.
    Utilizing a game loop system similar to pygame, you are able to abstract elements of your game away from the game loop as much as you'd like.
    """,
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
        }
    ),
    setup_requires=['wheel'],
    packages=find_packages(exclude=[
        "./Runespoor/core.cp311-win_amd64.pyd",
        "./Runespoor/core.cpp"
        "./test.py"
    ]),
    include_package_data=True,
    package_data={
        MODULE_NAME: [
            'LICENSE',
            '*.pyi',
            '*.glsl',
            'requirements.txt',
            'readme.md',
            '*.h'
        ]
    },
)