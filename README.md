# TS2 Heap Cap Remover
## About
An experimental patch for The Sims 2 based on the findings of my MSc [dissertation](A%20Recipe%20For%20Pink%20Soup.pdf), which (hopefully) fixes the issue of the 'pink soup' bug
that frequently occurs when playing the game on 64-bit versions of Windows.

Made for use with The Sims 2: Ultimate Collection and [Sims2RPC](https://modthesims.info/d/648220/sims2rpc-modded-sims-2-launcher-for-mansion-and-garden.html) with the 4 GB
patch applied.

## Explanation
Despite the common belief that the 'pink soup' bug is related to texture memory calculation mishaps, its cause is actually due to an intentional failsafe that prohibits shaders
being linked to objects that are stored in memory at addresses above a certain value &mdash; a limitation of the 32-bit architecture The Sims 2 was developed for.

The maximum value of an unsigned 32-bit integer is 4,294,967,296, which is equivalent to approximately 4GB. Because of this constraint, address space was split 50/50 on 32-bit
Windows, with 2GB reserved for the kernel and the remaining 2GB for applications.

To prevent the game dangerously operating on data within kernel space, a check was implemented in a function (found at address `0x00D947F3` in the game's binary) that
tests whether the address of the object being processed during shader linkage is greater than `0x7FFFFFFF` (~2GB), and if so, forces the process to fail, culminating in
the appearance of the 'pink soup'.

This presents an issue when the game is patched to be Large Address Aware (also known as a 4GB patch) on modern 64-bit Windows, as the range of addressable memory is
expanded from `0x00000000`&ndash;`0x7FFFFFFF` to `0x00000000`&ndash;`0xFFFFFFFF`. Although this allows the game to allocate data into a larger range of addresses
(and it is freely doing so), the built-in failsafe prohibits it from fully processing anything stored here.

This plugin alleviates these restrictions, with two separate versions offering similar functionality, but differing in their implementation:
- Version 1: The more 'brute-force' method, which patches out the failsafe entirely &mdash; the game will perform the same operations on an object stored above `0x7FFFFFFF`
  as it would on an object stored at addresses of `0x7FFFFFFF` or below.
- Version 2: A potentially 'safer' option that keeps the failsafe intact, but raises the point at which it triggers to addresses above `0xCFFFFFFF` (~3.5GB memory usage).
  *In theory*, this should allow the 'pink soup' to act as an indicator of high memory usage when it appears, giving you enough time to restart the game before it inevitably
  crashes. Realistically, however, I have never seen the 'pink soup' occur once this adjusted threshold had been passed, even when the failsafe trigger point
  was only marginally increased to affect addresses above `0x8FFFFFFF`. This may imply that all objects are stored in memory early during loading, meaning
  their addresses are never likely to reach a particularly high value, but ultimately, this makes it difficult for me to say how useful this version of the plugin truly is.

## Things To Note
- Even though the annoyance of the 'pink soup' has effectively been eliminated, it is important to keep in mind that this does not mean it is now possible to play with
  a greater amount of custom content, or with highly decorated neighbourhoods/lots etc.

  Because the game is a 32-bit program, it is still architecturally limited to a maximum address of `0xFFFFFFFF` if patched to be Large Address Aware (translating to
  a maximum memory usage of around 4GB), regardless of the presence of these checks or the hardware or operating system it is being played on. As mentioned previously,
  the game will simply crash when this limit is reached, which may lead to save corruption if this happens at an inopportune moment, such as during saving/loading.

- An identical piece of failsafe logic is found within the function at address `0x00D948B6`. During my extensive debugging of the game for this project, this set of checks was
  never triggered, but it is possible that they may be executed, causing the 'pink soup' as a result. Code has been left within the v1 plugin to remove these checks but has
  been commented out, as it is hard to test due to it never being hit.
- This was my first attempt at both writing code in C++, and creating a plugin that hooks into a program &mdash; I would be very surprised if I hadn't made a mistake somewhere
  or done something in a suboptimal manner.

## Thanks
[LazyDuchess](https://github.com/LazyDuchess), whose hooking library and various TS2 plugins I used as the basis/template for my code.