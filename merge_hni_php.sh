#! /bin/sh

printf "<?hh\n" > ext_event.php
tail -q -n +2 src/*php src/types/*php src/exceptions/*php >> ext_event.php
