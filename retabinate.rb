require 'ftools'

THIS_DIR = File.expand_path(File.dirname(__FILE__))
BACKUP_DIR = "retabinate_backup"

def retabinate_file(filename, backup)
    this_file = File.join(THIS_DIR, filename)

    puts "Retabinating #{filename}"
    data = File.read(this_file)
    File.copy(this_file, backup)

    data.gsub!(/\t/, "    ")
    f = File.open(this_file, "w")
    f.write(data)
    f.close
end

def retabinate_dir(dirname)
    this_dir = File.join(THIS_DIR, dirname)

    backup_dir = File.join(THIS_DIR, BACKUP_DIR, dirname)
    unless File.exists?(backup_dir)
        Dir.mkdir(backup_dir)
    end

    puts "Retabinating all files in #{dirname} (backing up in #{backup_dir})"
    eligible = Dir.entries(this_dir).reject { |i| i[0,1] == "." || i.match(BACKUP_DIR) }
    eligible.each do |entry|
        this_file = File.join(dirname, entry)
        if File.directory?(this_file)
            retabinate_dir(this_file)
        elsif entry.match(/\.h/) || entry.match(/\.c/)
            retabinate_file(this_file, backup_dir)
        end
    end
end

retabinate_dir(".")
