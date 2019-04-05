require "math"
require "os"


function get_name()
  return "Steven 'Boy Genius' Savold\n\n"

end

function turn()

  x = math.random(0,99)
  y = math.random(0,3)

  local complete, my_sqr, num_edges, top, bottom, left, right = sqr_info(x)

  return x, y
end


function i_win()
  io.write("\nI told you that your code was wrong!\n")
end


function i_lose()
  io.write("\nMusta forgot to include some header files...\n\n")
end

math.randomseed(os.time())
