project('gooroom-feedback-client', 'c', version: '1.0.0')

cc = meson.get_compiler('c')

gfb_gettext_package = meson.project_name()

gfb_prefix = get_option('prefix')
gfb_bindir = join_paths(gfb_prefix, get_option('bindir'))
gfb_datadir = join_paths(gfb_prefix, get_option('datadir'))
gfb_localedir = join_paths(gfb_prefix, get_option('localedir'))
gfb_schemadir = join_paths (gfb_datadir, 'glib-2.0', 'schemas')
gfb_desktopdir = join_paths(gfb_datadir, 'applications')

gnome = import('gnome')
i18n = import('i18n')
pkg = import('pkgconfig')

config_h = configuration_data()

#i18n
config_h.set_quoted('GETTEXT_PACKAGE', gfb_gettext_package)
config_h.set_quoted('LOCALEDIR', gfb_prefix / gfb_localedir)

common_flags = [
  '-DHAVE_CONFIG_H',
]

add_project_arguments(common_flags, language: 'c')

gtk_dep = dependency('gtk+-3.0')
glib_dep = dependency('glib-2.0')
gio_dep = dependency('gio-2.0')
curl_dep = dependency('libcurl')
json_c_dep = dependency('json-c')

gooroom_feedback_client_sources = [
  'src/gooroom-feedback-main.c',
  'src/gooroom-feedback-utils.c',
  'src/gooroom-feedback-app-window.c',
  'src/gooroom-feedback-write-dialog.c',
  'src/gooroom-feedback-list-row.c'
]

gooroom_feedback_client_sources += gnome.compile_resources(
  'gooroom-feedback-client-resources',
  'resources/gooroom-feedback-client.gresource.xml',
  source_dir: 'resources'
)

gooroom_feedback_client = executable(
  'gooroom-feedback-client',
  gooroom_feedback_client_sources,
  dependencies: [gtk_dep, glib_dep, gio_dep, curl_dep, json_c_dep],
  install: true,
  install_dir: gfb_bindir
)

po_dir = join_paths(meson.source_root(), 'po')

subdir('data')
subdir('po')

configure_file(
  output: 'config.h',
  configuration: config_h,
)

meson.add_install_script('meson_post_install.py', gfb_datadir)
