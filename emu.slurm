#!/bin/bash -e

base="emu.slurm.$SLURM_JOB_ID.$SLURM_STEP_ID"
[ "$SLURM_PROCID" -eq 0 ] && echo "$base.*"

exec "${BASH_SOURCE%/*}/emu" -n "${SLURM_LOCALID?}" "$@" > "$base.$SLURM_PROCID"
