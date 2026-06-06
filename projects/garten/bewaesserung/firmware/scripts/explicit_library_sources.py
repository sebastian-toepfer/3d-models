from os.path import normpath

Import("env")


def as_list(value):
    if isinstance(value, (list, tuple)):
        return value
    return [line.strip() for line in str(value).splitlines() if line.strip()]


for include_dir in as_list(env.GetProjectOption("custom_explicit_library_include_dirs", [])):
    env.Append(CPPPATH=["$PROJECT_DIR/%s" % include_dir])

for src_dir in as_list(env.GetProjectOption("custom_explicit_library_sources", [])):
    build_name = normpath(src_dir).replace("/", "__")
    build_dir = "$BUILD_DIR/explicit-library-sources/%s" % build_name
    env.BuildSources(build_dir, "$PROJECT_DIR/%s" % src_dir)
