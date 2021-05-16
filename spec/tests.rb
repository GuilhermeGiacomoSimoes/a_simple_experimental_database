describe 'database' do
  def run_script(commands)
    raw_output = nil
    IO.popen("./REPL", "r+") do |pipe|
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
      "insert 1 user1 person1@example.com",
      "select",
      ".exit",
    ])
    expect(result).to match_array([
      "db> Executed ",
      "db> (1, user1, person1@example.com)",
      "Executed ",
      "db> ",
    ])
  end

  it 'tests TABLE_MAX_ROWS' do
    script = (1..1401).map do |i|
      "insert #{i} person#{i} person#{i}@gmail.com"
    end
    script << ".exit"
    result = run_script(script)
    expect(result[-2]).to eq("db> Error: Table full. ")
  end

  it 'allows inserting strings that are the maximum length' do
    long_username = "a"*33
    long_email = "a"*256
    script = [
      "insert 1 #{long_username} #{long_email}",
      "select",
      ".exit",
    ]
    result = run_script(script)
    expect(result).to match_array([
      "db> String is too long",
      "db> Executed ",
      "db> ",
    ])
  end
end
