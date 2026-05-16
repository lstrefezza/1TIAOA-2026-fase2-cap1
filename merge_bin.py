Import("env")

def after_build(source, target, env):
    build_dir = env.subst("$BUILD_DIR")
    progname = env.subst("${PROGNAME}")
    # Merge bootloader, partition table and app binary
    cmd = (
        f'"{env.subst("$PYTHONEXE")}" '
        f'"{env.subst("$OBJCOPY")}" '
        f'--chip esp32 merge_bin -o "{build_dir}/merged.bin" '
        f'--flash_mode dio --flash_freq 40m --flash_size 4MB '
        f'0x1000 "{build_dir}/bootloader.bin" '
        f'0x8000 "{build_dir}/partitions.bin" '
        f'0x10000 "{build_dir}/{progname}.bin"'
    )
    env.Execute(cmd)

# Run after building the firmware binary
env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", after_build)
