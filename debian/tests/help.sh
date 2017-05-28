#!/bin/bash

set -e

# At least check we can execute the main binaries
virt-sandbox --help
virt-sandbox-image --help
