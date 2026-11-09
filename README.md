# TS2 Memory Address Cap Remover
## About
A patch for The Sims 2 based on the findings of my MSc [dissertation](A%20Recipe%20For%20Pink%20Soup.pdf), which aims to fix the issue of the 'pink soup'
bug that frequently occurs when playing the game on 64-bit versions of Windows.

## Requirements
- The Sims 2: Ultimate Collection <ins>**OR**</ins> The Sims 2 disc version with all EPs and SPs.

- [Sims2RPC](https://modthesims.info/d/648220/sims2rpc-modded-sims-2-launcher-for-mansion-and-garden.html) <ins>**OR**</ins>
  [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader).

Those playing the Legacy Collection should use [TS2 Extender](https://github.com/LazyDuchess/TS2-Extender), which incorporates the fix from this mod.

## Explanation
Despite the common belief that the 'pink soup' bug is related to texture memory calculation mishaps, its cause is actually due to an apparent failsafe that prohibits shaders
being linked to objects that are stored in memory at addresses above a certain value &mdash; a limitation of the 32-bit architecture The Sims 2 was developed for.

The maximum value of an unsigned 32-bit integer is 4,294,967,295, which is equivalent to approximately 4GB. Because of this constraint, address space was split 50/50 on 32-bit
Windows, with 2GB reserved for the kernel and the remaining 2GB for applications.

To prevent the game dangerously operating on data within kernel space, a check was implemented in a function (found at address `0x00D947F3` in the game's binary) that
tests whether the address of the object being processed during shader linkage is greater than `0x7FFFFFFF` (~2GB), and if so, forces the process to fail, culminating in
the appearance of the 'pink soup'.

This presents an issue when the game is patched to be Large Address Aware (also known as a 4GB patch) on modern 64-bit Windows, as the range of addressable memory is
expanded from `0x00000000`&ndash;`0x7FFFFFFF` to `0x00000000`&ndash;`0xFFFFFFFF`. Although this allows the game to allocate data into a larger range of addresses
(and it is freely doing so), the failsafe prohibits it from fully processing anything stored here.

This plugin alleviates these restrictions, with two separate versions offering similar functionality, but differing in their implementation:
- <ins>**Version 1**</ins>: The more 'brute-force' method, which patches out the failsafe entirely &mdash; the game will perform the same operations on an object stored above
  `0x7FFFFFFF` as it would on an object stored at addresses of `0x7FFFFFFF` and below.

- <ins>**Version 2**</ins>: A potentially 'safer' option that keeps the failsafe intact, but raises the point at which it triggers to addresses above `0xCFFFFFFF` (~3.5GB
  memory usage). *In theory*, this should allow the 'pink soup' to act as an indicator of high memory usage when it appears, giving you enough time to restart the game
  before it inevitably crashes.

## Things To Note
Even though the annoyance of the 'pink soup' has effectively been eliminated, it is important to keep in mind that this does not mean it is now possible to play with
a greater amount of custom content, or with highly decorated neighbourhoods/lots etc.

Because the game is a 32-bit program, it is still architecturally limited to a maximum address of `0xFFFFFFFF` if patched to be Large Address Aware (translating to
a maximum memory usage of around 4GB), regardless of the presence of these checks or the hardware or operating system it is being played on &mdash; as mentioned previously,
the game will simply crash when this limit is reached.

## Installation
**Sims2RPC Users:**

1. Download one of the two versions of the plugin found under the [Releases](https://github.com/spockthewok/TS2MemCapRemover/releases) section of this repository.
2. Move the downloaded plugin to the `\TSBin\mods` directory, found wherever you have the Sims 2 installed to. For example, on my machine, the plugin would be moved to:

   `E:\Games\The Sims 2\Fun with Pets\SP9\TSBin\mods`

**Ultimate ASI Loader Users:**

1. Download Ultimate ASI Loader from [here](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases/download/Win32-latest/dsound-Win32.zip).
2. Extract `dsound.dll` from the zip file and place it in the game's `\TSBin` directory. On my machine, it would go here:

   `E:\Games\The Sims 2\Fun with Pets\SP9\TSBin`
3. Download one of the two versions of the plugin found under the [Releases](https://github.com/spockthewok/TS2MemCapRemover/releases) section of this repository.
4. Move the plugin to the same `\TSBin` directory Ultimate ASI Loader was extracted to.

## Community Testimonials
| Before | After |
| :----: | :---: |
| <img src="img/RachPinkBefore.png" width="450"> | <img src="img/RachPinkAfter.png" width="450"> |
| <img src="img/GummiPinkBefore.png" width="450"> | <img src="img/GummiPinkAfter.png" width="450"> |
| <img src="img/AlexPinkBefore.png" width="450"> | <img src="img/AlexPinkAfter.png" width="450"> |
| <img src="img/BethPinkBefore.png" width="450"> | <img src="img/BethPinkAfter.jpg" width="450"> |
| <img src="img/ReveriePinkBefore.png" width="450"> | <img src="img/ReveriePinkAfter.png" width="450"> |

### Tumblr Success Stories

[12Raben](https://www.tumblr.com/12raben/814509102470496256/one-of-the-reasons-why-i-stopped-playing)

[Blissful-Indigo](https://www.tumblr.com/blissful-indigo/814713731385819136/my-experience-with-spockthewoks-pink-soup-patch)

[EJ Sims](https://www.tumblr.com/ejsims/814901132709249024/in-the-10-years-since-ive-been-playing-ts2-again)

[eleysims](https://www.tumblr.com/eleysims/814589060560650240)

[LordCrumps](https://www.tumblr.com/lordcrumps/814605472484229120)

[nerianasims](https://www.tumblr.com/nerianasims/814812643614441472/please-dont-be-pink-please-dont-be-pink-please)

## Thanks
[LazyDuchess](https://github.com/LazyDuchess), whose hooking code and various TS2 plugins I used as the basis/template for my own code.

[TeaAddictYT](https://github.com/TeaAddictYT), for kindly gathering the before/after pictures seen above.

Special thanks also goes to The Sims 2 community as a whole &mdash; not only for producing many of the resources and tools necessary to complete this project,
but also for being so welcoming and providing such positive feedback towards my work.