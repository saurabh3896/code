#!/usr/bin/env ruby

require 'active_record'
require 'yaml'
require 'pp'

dbconf = YAML::load(File.open('db.yaml'))
ActiveRecord::Base.establish_connection(dbconf)

<<<<<<< HEAD
class User < ActiveRecord::Base
  self.table_name = 'user'
end

puts User.count

#Dept.find(:all).each { |s| puts s.dname }
=======
class App < ActiveRecord::Base
  self.primary_key = 'id'
  self.table_name  = 'apps'
end

puts App.count

pp App.find_each { |s| pp s }
# pp App.find_by(name: "QQ")

# App.find(:all).each { |s| puts s }
>>>>>>> f8350de0f2827b8bc2c8dc0cbef5ec823b85f5c3

