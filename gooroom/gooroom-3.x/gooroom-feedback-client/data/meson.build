#desktop = i18n.merge_file(
#  input: 'gooroom-feedback-client.desktop.in',
#  output: 'gooroom-feedback-client.desktop',
#  type: 'desktop',
#  po_dir: '../po',
#  install: true,
#  install_dir: '/usr/share/applications'
#)

desktop = 'gooroom-feedback-client.desktop'

i18n.merge_file(
       desktop,
         type : 'desktop',
        input : 'gooroom-feedback-client.desktop.in',
       output : desktop,
       po_dir : po_dir,
       install: true,
  install_dir : gfb_desktopdir
)

install_data('gooroom-feedback-client-problem-symbolic.svg',
             'gooroom-feedback-client-suggestion-symbolic.svg',
  install_dir: join_paths(gfb_datadir, 'icons', 'hicolor', 'scalable', 'status')
)

install_data('gooroom-feedback-client.svg',
  install_dir: join_paths(gfb_datadir, 'icons', 'hicolor', 'scalable', 'apps')
)

install_data (
  'kr.gooroom.FeedbackClient.gschema.xml',
  install_dir : gfb_schemadir
)

install_data (
  'GOOROOM_FEEDBACK_SERVER.crt',
  install_dir : '/etc/ssl/certs'
)
