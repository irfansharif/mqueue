require "bundler/gem_tasks"
require "rake/extensiontask"
require "rake/testtask"

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList['test/**/*_test.rb']
end

Rake::ExtensionTask.new "mqueue" do |ext|
  ext.lib_dir = "lib/mqueue"
end

task :default => :test
