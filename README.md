# OP-TEE sanity testsuite for Zephyr

This application contains source code for the test suite (xtest) used to test the
OP-TEE project on Zephyr.

Source code was ported from [optee_test] tag v3.18.0.

All official OP-TEE documentation has moved to http://optee.readthedocs.io.

[optee_test]: git@github.com:OP-TEE/optee_test.git

# Build instructions

This application was tested with the following boards:
- rcar_h3ulcb_ca57;
- rcar_spider;
- rcar_salvator_xs_m3.

It is represented as Zephyr application which can be started on the target platform.
Build instructions:
```
 west init -l ./
 west update
 # Build for the native board
 west build -b <board> -p always -- -DTA_DEPLOY_DIR=$(pwd)/prebuilt
 # .. or build with xen_dom0 SHIELD
 west build -b <board> -p always -- -DSHIELD=xen_dom0 -DTA_DEPLOY_DIR=$(pwd)/prebuilt
```
The following define indicates that this application should be run as Xen Domain-0:
> SHIELD=xen_dom0

The next define sets the directory with prebuilt TA to build ta_table for tee_supplicant (see ./scripts/gen_ta_src.py script help for details):
> TA_DEPLOY_DIR=$(pwd)/prebuilt

Those prebuilt TAs can be get from the original [optee_test] build directory and from optee_os package.
If TA's weren't provided, then supplicant will expect those TA's to be embedded into the OP-TEE Early TA storage.
