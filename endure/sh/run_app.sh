#!/usr/bin/env bash
# __author__ = 'harshal'
eval "$(conda shell.bash hook)"
# conda activate python3.6
DRIVER_PATH='robust-lsm-trees.py'
python ${DRIVER_PATH}
conda deactivate
