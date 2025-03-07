# NEWER Super Mario Bros. Wii - Translatable Hack
## Translations

The Majority of NewerSMBW's custom texts are either loaded from a non-language
specific file, hardcoded, or stored inside the layout file.
This causes NewerSMBW based mods to be either single language or english only,
depending on the type of text.
This version of NewerSMBW moves all the game text into the Message.arc file,
making it completely translatable into other languages.

### Changes from NewerSMBW

* Incorportated [Wakanameko's UTF LevelInfo Mod][utf].
* Moved LevelInfo.bin and Messages.bin to the `Language/Newer` folder.
* Added new entries for all of Newer's text into the Message.arc file.
* Changed Layout code to load strings from the Message.arc file.
* Koopatlas now loads world names from the Message.arc file.

### Installation

Add the "Lang" folder to your NewerSMBW folder. Delete the "Message" and "Font" folders.
Add the Language patches from the riivolution xml to your riivolution xml.
NOTE: currently the only supported languages are:
* European English

### Adding a Translation

Pull requests to add or improve translations will gladly be accepted.
LevelInfo must be edited using [Wakanameko's UTF LevelInfo Editor][utfed].

For BMG editing, refer to the BMG INFO.txt file.

Credits translations are not supported at this time.

## Original Readme

### Preamble

Well, here it is, the thing you've been awaiting for... years. Yep.
My original plans were to majorly clean up and reorganise the codebase and
tools, because I really wasn't satisfied with the current state of them.

Unfortunately, this didn't quite pan out - as much as I'd like to have done
this, getting everything into the state I wanted would require months upon
months of work - something that's a bit difficult to do nowadays, since Newer
is done and I don't wish to devote yet more of my life to projects related to
NSMBW.

So... I've decided to dump the source code here so other people can do
something with this. Much of it isn't really in a usable state - but I'm
releasing it in case someone else wants to take up development and work on
getting everything polished and ready.

*-Ninji, 22nd November 2013*

### Licensing

The Newer SMBW source code and tools are released under the MIT license.
See the `LICENSE` file in this repository for full details.

Copyright (c) 2010-2022 Newer Team

### Requirements

- Windows or OS that runs [Wine]
- [devkitPPC][dkp]
- Python 3.x
- Python libraries: PyYAML, pyelftools
- NXP ['CodeWarrior Special Edition'][cw] for MPC55xx/MPC56xx v2.10
  - If this direct link doesn't work, the original page is
    [available on the Internet Archive][cwIA].
  - The installer does not work with Wine; you may use `extractCW.sh`
    from the root of the repo in the same directory as the installer to
    extract the necessary files instead (requires cabextract).
  - You only need the Command Line Tools component in the custom
    install configuration.
  - After installing it, you need `license.dat` from the installation root,
    and the DLL and EXE files inside `PowerPC_EABI_Tools/Command_Line_Tools`.

Full setup instructions are included at the bottom of this readme.

### What's Here

#### Kamek

All the Newer hacks are inside the Kamek folder in this repository because
that's how our files have always been organised, but technically, Kamek is
really just a simple build system for calling the tools to compile/assemble
each source file and putting together an output file you can inject into your
game.

Unlike most typical build systems, though, there's a bunch of features
specific to game hacks like Newer:

- Kamek is configured through a game-specific project file; each project
  pulls in various modules, which can each contain C++ and assembly source
  files, and hooks
- Hooks are different types of patches that are used to inject things into the
  game: patch static code/data in RAM, overwrite pointers with a reference to
  a symbol exported from Kamek code, inject branch instructions, ...
- Memory addresses in the linker script and used by hooks are automatically
  converted to match all supported game versions - Kamek uses the original
  version of the PAL/European NSMBW release as a reference.
- The compiled output is converted to a specific format expected by the
  Newer loader.

There's also a few things I'm not quite happy with...

- The projects/modules system turned out to be rather messy in Newer
- I think there's some features which just aren't used any more because the
  Newer loader changed over time.
- I wanted to add some NSMBW-specific features, like an easy way to inject
  new actors and sprites without having to modify the data structures using
  very-easy-to-mess-up Kamek hooks, but I never got round to this

#### Newer Hacks

The stuff that makes this game tick. Thousands of lines of C++ code, a
smattering of assembly, tons of patches, and a lot of blood, sweat and
tears. (No Ninjis or Chronometrics were harmed in the creation of these.
Well. Not harmed THAT much, anyway.)

Kamek projects and the code for Newer and all of the specials are included.

#### Bootstrap

This is the in-game half of Kamek - it's a simple loader that links the
custom code generated by Kamek into the game after the rels.

Unlike the rest, this hasn't been updated to build with modern dependencies,
so it requires Python 2 and has only been finangled into working with WSL
just so we could add support for a couple extra regions in an update.
So the setup for building this is a complete disaster.

Thankfully, since it works perfectly fine (for the most part), and now
supports every variation of NSMBW that was released for the Wii,
there's not too much of a reason to deal with this. It's mostly here
for posterity's sake.

### So how DO I use this junk?

Good question.

I could probably write an entire book (or two) documenting how to set up the
tools, the internals of the NSMBW game engine, and everything I added in Newer.
But... do you have any idea how long that would take?

The setup procedures are included below this section.

As for the game engine APIs - I'd love to document these, but it would simply
take way, way too long. Let us know if you have any technical questions
involving these: you can send your query to the email [admin@newerteam.com](mailto:admin@newerteam.com)

## Installation

### Setup

- First install Python 3. If on Windows, ensure it is added to PATH
  upon installation. Next, run:

  - On Windows:
    `py -3 -m pip install pyyaml pyelftools`

  - On other OSs:
    `python3 -m pip install pyyaml pyelftools`

- Install DevkitPro's devkitPPC. Their official instructions for
  this are found [here][dkp].

- The last dependency is NXP ['CodeWarrior Special Edition'][cw]
  for MPC55xx/MPC56xx v2.10. You will also need [Wine] if you
  are running an OS other than Windows.

  - NOTE: the installer is known to fail with Wine.
    You may extract the files from it with ``extractCW.sh``
    on the root of the repo (which requires cabextract,
    available from most package managers).

  - CodeWarrior's tools are by default installed to:
  
  `C:\Program Files (x86)\Freescale\CW for MPC55xx and MPC56xx 2.10\PowerPC_EABI_Tools\Command_Line_Tools`

  - Copy the files inside to `Kamek/tools/cw` - Kamek assumes they
    are there.

  - Also, on Windows, make sure the following system variable is added
    to your environment variables, with the name `LM_LICENSE_FILE`:

  `C:\Program Files (x86)\Freescale\CW for MPC55xx and MPC56xx 2.10\license.dat`

  - The ``license.dat`` file may alernatively be copied to
    `Kamek/tools/cw` if you are using Wine.

### Usage

There is a set of scripts in the Kamek folder that invoke
`makeGame.py` with different arguments - one for each separate mod:

- makeAnother (Another Super Mario Bros. Wii)
- makeHolidaySpecial (Newer Holiday Special)
- makeSummerSun (Newer Summer Sun)
- makeNewer (Newer Super Mario Bros. Wii)
- makeFL (Newer Falling Leaf)

On Windows, run the batch versions - you can just double-click them.

On macOS, Linux, or other Unix-like systems, use the bash versions.
Open a terminal in the same directory and run them like:

`./makeNewer`

The resulting output is placed in a folder called `Build_X`
(where X is the name of the Kamek project). These files go in
`/NewerRes` on the game disc (all the Newer mods include this folder
in the Riivolution patch).

[cw]: http://cache.nxp.com/lgfiles/devsuites/PowerPC/CW55xx_v2_10_SE.exe?WT_TYPE=IDE%20-%20Debug,%20Compile%20and%20Build%20Tools&WT_VENDOR=FREESCALE&WT_FILE_FORMAT=exe&WT_ASSET=Downloads&fileExt=.exe
[cwIA]: http://web.archive.org/web/20160602205749/http://www.nxp.com/products/software-and-tools/software-development-tools/codewarrior-development-tools/downloads/special-edition-software:CW_SPECIALEDITIONS
[dkp]:
https://devkitpro.org/wiki/Getting_Started
[Wine]:
https://www.winehq.org/
[utf]:
https://github.com/wakanameko/NewerSMBW_UTF-8/tree/cw
[utfed]:
https://github.com/Developers-Collective/Level-Info-Editor_UTF
