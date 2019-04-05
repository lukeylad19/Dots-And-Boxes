require ("math")
require ("os")


function get_name()
  return "Lukas 'The Prodigal Child' Mallory\n\n"
end

function turn()
    local complete, my_sqr, num_edges, top, bottom, left, right = sqr_info(x)

    if complete then
      if x == 0 then
        x = x + 2
      elseif x%2 == 0 then
        if x ~= 98 then
          x = x + 2
        else
          x = 1
        end
      else
        x = x + 2
      end
    end

    if not complete then
      if not top then
        y = 0
      elseif not bottom then
        y = 1
      elseif not left then
        y = 2
      elseif not right then
        y = 3
      end
    end

    return x, y
end

function i_win()
  io.write("\nYou lose Carl Miller! !-_-!\n")
end


function i_lose()
  io.write("\nREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n\n")
end

x = 0
y = 0
