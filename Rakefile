task default: [:generate, :test, :build, :run]

puts "Builder => Welcome to the Marvelous Realm of Eärndayl"

desc 'Cleans all temporary, build, and intermediate files'
task :clean do
  puts "Cleaning"
  r = system('ninja -t clean')
  exit! unless r
end

desc 'Generates content and code for the game'
task :generate do
  puts "Generating code"
end

desc 'Builds the game'
task :build do
  puts "Building game"
  r = system('ninja main')
  exit! unless r
end

desc 'Runs the tests'
task :test do
  puts "Running tests"
  r = system('ninja test run-test')
  exit! unless r
end

desc 'Runs the game'
task run: [ :build ] do
  puts "Running game"
  r = system('build/main')
  exit! unless r
end
