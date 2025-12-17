# TS2 Heap Cap Remover
## About
An experimental patch for The Sims 2 based on the findings of my MSc [dissertation](A%20Recipe%20For%20Pink%20Soup.pdf), which (hopefully) fixes the issue of the 'pink soup' bug
that frequently occurs when playing the game on 64-bit versions of Windows.

Made for usage with The Sims 2: Ultimate Collection and [Sims2RPC](https://modthesims.info/d/648220/sims2rpc-modded-sims-2-launcher-for-mansion-and-garden.html) with the 4 GB patch applied.

## Explanation
Despite the common belief that the 'pink soup' bug is related to texture memory calculation mishaps, its cause is actually due to an intentional failsafe that prohibits shaders
being linked to objects that are stored on the heap in addresses above a certain value &mdash; a limitation of the 32-bit architecture The Sims 2 was developed for.

The maximum value of an unsigned 32-bit integer is 4,294,967,296, which is equivalent to approximately 4 GB. Because of this constraint, address space was split 50/50 on 32-bit Windows,
with 2 GB reserved for the kernel and the remaining 2 GB for any applications.

To prevent the game dangerously operating on data within kernel space, a check was implemented in a function (found at address `0x00D947F3` in the game's binary) that
tests whether the address of the object being processed during shader linkage is greater than `0x7FFFFFFF` (~2 GB), and if so, forces the process to fail, culminating in
the appearance of the 'pink soup'.

While the game makes heavy use of 32-bit Windows functions, modern releases of Windows redirect these to updated, 64-bit versions of the same functions when they are called. This includes
the heap allocation functionality that The Sims 2 utilises, meaning that although Windows is now allowing the game to allocate data into a much larger range of addresses (and the game is
freely doing so), the built-in failsafe prohibits it from fully processing anything stored here.

This plugin therefore simply patches out these checks, allowing shaders to be correctly linked to objects stored in addresses above `0x7FFFFFFF`.

## Potential Issues
- Because the game is a 32-bit program, it is still architecturally limited to a maximum address of `0xFFFFFFFF` if patched to be Large Address Aware (also known as a 4 GB patch), 
  or `0x7FFFFFFF` if not, regardless of the presence of these checks or the hardware or operating system it is being played on. Consequently, the game will likely simply crash when this
  limit is reached, which may lead to save corruption if this happens at an inopportune moment, such as during saving/loading.

  To account for this, an alternative solution is proposed on pages 52&ndash;53 of my dissertation, that suggests doubling the address cap based on whether the game is patched to be Large 
  Address Aware. This means that the 'pink soup' would still occur once this expanded cap is reached, but its appearance would be less frequent than it would in an unpatched game, and may
  also help avert any crashes described above (or least delay them, allowing the player to exit the game safely once the 'pink soup' has appeared).

- An identical piece of failsafe logic is found within the function at address `0x00D948B6`. During my extensive debugging of the game for this project, this set of checks were
  never triggered, but it is possible that these may be executed, causing the 'pink soup' as a result. Code has been included within the plugin to remove these checks but has been
  commented out, as it is difficult for me to test due to it never being hit.
- This was my first attempt at both writing code in C++, and creating a plugin that hooks into a program &mdash; I would be very surprised if I hadn't made a mistake somewhere or done something
  in a suboptimal manner.

## Thanks
[LazyDuchess](https://github.com/LazyDuchess), whose hooking library and various TS2 plugins I used as the basis/template for my code.
