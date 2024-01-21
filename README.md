# Marvelous Realm of Eärndayl

January 2024: this project is a work in progress of a text adventure written in C99.

The *Marvelous Realm of Eärndayl* is also a working title, inspired by [The Wonderful World of Eamon](https://en.wikipedia.org/wiki/Eamon_(video_game)), a game I played as a kid. Nine-year-old me would visit the local library with some blank diskettes and copy public domain Eamon disks so that I could play them on my APPLE II. The game was written in Applesoft BASIC, which inspired me to learn BASIC and write my own text adventures. I remember poring over the spiral-bound Applesoft BASIC manual in the dim basement with the green phosphorescent glow of a monochrome monitor in front of me, trying to decipher PEEK and POKE and GOSUB.

In any case, this project is an homage to that experience. I never wrote that text adventure in full as a child or an adult (despite many false starts), and decided I wanted to correct for that.

I chose the C programming language in tribute to the old-school nature of the game. I wanted to use a language that was in use at the time. I considered Applesoft BASIC (or another variant of unstructured BASIC), but this was limiting for a variety of reasons. I considered Pascal and Fortran, but I am more comfortable in C. C is also the most portable language available, which seemed like an added bonus. I opted for C99 as a reasonable baseline.

Note that this is not a retro computing project. The goal is to create a game that harkens back to olden times but builds and runs happily on modern computers without the need of emulators.

## Project Goals

- Write an homage to WWoE that builds and runs on a wide number of modern machines.
- Written in C99 with zero dependencies beyond the codebase itself.
- Avoid all dynamic memory allocation.
- Provide a good set of baseline tests.

## License

The bulk of the code in this repo was written from scratch by Matthew MacGregor, and is shared under the MIT license. See LICENSE for details.

Some code (and a great deal of inspiration) draws from Ruud Helderman's <r.helderman@hccnet.nl> [excellent example](https://helderman.github.io/htpataic/htpataic01.html) of a text adventure in C, and also carries the MIT license. The more that I work on the project the more I have diverged on my own path.

Minunit is liberally licensed by John Brewer, and comes with NO WARRANTY. See `minunit.h` for more information.

## Development Prerequisites

To build the C source code:
- Install [ninja](https://ninja-build.org/) for your platform.

For code generation:
- Ruby 3.2.2+

## Building the Content with Rake

To build code and content, run the `rake` command.

Other tasks are available. Run `rake --tasks` for more information.

I decided to use Rake as the high-level build environment for this project, since I also plan to use Ruby to generate some of the content files (see `game/game_data.c` as an example). Rake is an excellent task runner and seemed like a great fit.

## Building the C Source Code

To simply compile the C source code, run the `ninja` command, or use the `rake` commands listed above.

Note: during development, the build.ninja file is for macOS/Linux/Unix systems only, and it is tuned for debugging and warnings.

## Running the Tests

I decided to make this project mostly test-driven, or failing that, at least provide a consistent set of tests for baseline functionality. This is useful for all of the usual reasons, but especially because it makes it easier for me to add functionality to the game without having to actually run the game.

To run the tests: `rake test` or `ninja test`. They will be run automatically with the default tasks as well.

The tests are using [minunit.h](https://jera.com/techinfo/jtns/jtn002) by [John Brewer](https://jera.com/jbrewer), which is literally just a few macros. It was the most minimal thing I could find and, so far, I haven't felt the need for anything else.