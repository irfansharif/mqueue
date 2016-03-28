# Extension configuration
require 'mkmf'

have_header('mqueue.h')
have_library('rt')

extension_name = 'mqueue'
dir_config(extension_name)
create_makefile(extension_name)
