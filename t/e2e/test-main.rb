#!/usr/bin/env ruby

describe 'database' do 

  before(:all) do 
    `rm database.db`
  end

  def run_script(commands)
    raw_output = nil
    IO.popen('./database.db', "r+") do |pipe|
      commands.each do |command|
        pipe.puts command
      end

      pipe.close_write

      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end



  it 'inserts and retrieves a row' do 
    result = run_script([
      "insert 1 guilherme", 
      "select 1"
      ".exit"
    ])
    expect(result).to match_array([
      "db> execute.",
      "db> (1, guilherme)", 
      "db>"
    ])
  end
end
