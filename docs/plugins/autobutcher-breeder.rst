autobutcher-breeder
===================

.. dfhack-tool::
    :summary: Retain high-potential breeders and butcher excess livestock.
    :tags: fort auto fps animals

This plugin is an attribute-selecting counterpart to `autobutcher`. It keeps
the same female/male and juvenile/adult population targets, but chooses excess
animals so the retained breeding stock has the strongest weakest physical
attribute.

For each animal, the breeder score is the minimum of its six physical
attribute potential values: strength, agility, toughness, endurance,
recuperation, and disease resistance. Lower-scoring animals are selected for
slaughter before higher-scoring animals within each reproductive-priority
group. Potential (``max_value``) is used instead of current ability so
juveniles and untrained animals can be compared fairly. Mental attributes are
not included since ordinary livestock may not have souls.

The plugin requires that you add target races to its watchlist. It has separate
settings from `autobutcher`. Do not enable both plugins for the same race: both
write the same slaughter flag, so whichever plugin processes a unit first can
determine the result.

Units are counted toward the target but protected from slaughter if they are:

* Named or nicknamed (for custom protection; you can use the `rename` ``unit``
  tool individually, or `zone` ``nick`` for groups)
* Caged, if and only if the cage is defined as a room (to protect zoos)
* Trained or marked for war or hunting training
* Chained, pregnant, or available for adoption

Untamed, undead, merchant, forest, and non-civilization units are ignored and
do not count toward the target.

Creatures who will not reproduce (because they're not interested in the
opposite sex or have been gelded) will be butchered before those who will.
Within equal breeder scores, older adults and younger children are butchered
first. Defaults are 1 male kid, 5 female kids, 1 male adult, and 5 female
adults. You may need a target above 1 for a reliable breeding population due
to asexuality. See `fix-ster` if this is a problem.

.. note::

    DFHack exposes each animal's observed attribute potential, but not a
    separate physical-attribute genotype. This plugin ranks breeding stock by
    those visible potential values; it does not guarantee that offspring will
    inherit them.

Usage
-----

``enable autobutcher-breeder``
    Start processing livestock according to the configuration. Note that
    no races are watched by default. You have to add the ones you want to
    monitor with ``autobutcher-breeder watch``, ``autobutcher-breeder target`` or
    ``autobutcher-breeder autowatch``.
``autobutcher-breeder autowatch``
    Automatically add all new races (animals you buy from merchants, tame
    yourself, or get from migrants) to the watch list using the default target
    counts.
``autobutcher-breeder noautowatch``
    Stop auto-adding new races to the watch list.
``autobutcher-breeder now``
    Run one processing cycle immediately.
``autobutcher-breeder target <fk> <mk> <fa> <ma> all|new|<race> [<race> ...]``
    Set target counts for the specified races:
    - fk = number of female kids
    - mk = number of male kids
    - fa = number of female adults
    - ma = number of male adults
    If you specify ``all``, then this command will set the counts for all races
    on your current watchlist (including the races which are currently set to
    'unwatched') and sets the new default for future watch commands. If you
    specify ``new``, then this command just sets the new default counts for
    future watch commands without changing your current watchlist. Otherwise,
    all space separated races listed will be modified (or added to the watchlist
    if they aren't there already).
``autobutcher-breeder ticks <ticks>``
    Change the number of ticks between scanning cycles when the plugin is
    enabled. By default, a cycle happens every 6000 ticks (about 8 game days).
``autobutcher-breeder watch all|<race> [<race> ...]``
    Start watching the listed races. If they aren't already in your watchlist,
    then they will be added with the default target counts. If you specify the
    keyword ``all``, then all races in your watchlist that are currently marked
    as unwatched will become watched.
``autobutcher-breeder unwatch all|<race> [<race> ...]``
    Stop watching the specified race(s) (or all races on your watchlist if
    ``all`` is given). The current target settings will be remembered.
``autobutcher-breeder forget all|<race> [<race> ...]``
    Unwatch the specified race(s) (or all races on your watchlist if ``all`` is
    given) and forget target settings for it/them.
``autobutcher-breeder [list]``
    Print status and current settings, including the watchlist. This is the
    default command if autobutcher-breeder is run without parameters.
``autobutcher-breeder list_export``
    Print commands required to set the current settings in another fort.

To see a list of all races, run this command:

    devel/query --table df.global.world.raws.creatures.all --search ^creature_id --maxdepth 1

Though not all the races listed there are tameable/butcherable.

.. note::

    Settings and watchlist are stored in the savegame, so you can have different
    settings for each save. If you want to copy your watchlist to another,
    savegame, you can export the commands required to recreate your settings.

    To export, open an external terminal in the DF directory, and run
    ``dfhack-run autobutcher-breeder list_export > filename.txt``.  To import, load your
    new save and run ``script filename.txt`` in the DFHack terminal.

Examples
--------

Keep at most 7 kids (4 female, 3 male) and at most 3 adults (2 female, 1 male)
for turkeys. Animals with lower breeder scores will be selected first. When
scores tie, the oldest adults and youngest kids will be selected first::

    autobutcher-breeder target 4 3 2 1 BIRD_TURKEY

Configure useful limits for dogs, cats, geese (for eggs, leather, and bones),
alpacas, sheep, and llamas (for wool), and pigs (for milk and meat). All other
unnamed tame units will be marked for slaughter as soon as they arrive in your
fortress::

    enable autobutcher-breeder
    autobutcher-breeder target 2 2 2 2 DOG
    autobutcher-breeder target 1 1 2 2 CAT
    autobutcher-breeder target 50 50 14 2 BIRD_GOOSE
    autobutcher-breeder target 2 2 4 2 ALPACA SHEEP LLAMA
    autobutcher-breeder target 5 5 6 2 PIG
    autobutcher-breeder target 0 0 0 0 new
    autobutcher-breeder autowatch
