# Functions
function walkdir(dir)
  d = abspath(dir)
  f = readdir(abspath(dir))
  for i in f
    #spaces = length(matchall(r"/", joinpath(dir,i)))
    #println(" "^ (spaces * 3),  joinpath(dir,i))
    if isdir(joinpath(dir,i))
      d = [d ; walkdir(joinpath(dir,i))]
    else
      d = [d ; joinpath(dir,i)]
    end
  end
  return d
end

findindir(dir,key) = filter(x->contains(x,key), walkdir(dir))

function readkeelrslt(filename)
  filestream = open(filename, "r")
  readuntil(filestream, "@data\n")
  matrix = Array(Float64,0,2)

  while !eof(filestream)
    strline = readline(filestream)
    strline = strip(strline)
    newrow = map(float64,split(strline,' '))'
    matrix = [matrix ; newrow]
  end

  return matrix
end

mse(y,ŷ) = sum((y-ŷ).^2)/size(y,1)
mad(y,ŷ) = sum(abs(y-ŷ))/size(y,1)
acc(eps,y,ŷ) = sum(abs(y-ŷ) .<= eps)/size(y,1)

using PyPlot
function processkeelrslt(filename)

  results = readkeelrslt(filename) # / 1e7
  stats = [
    mse(results[:,1],results[:,2])
    mad(results[:,1],results[:,2])
    acc(0.5,results[:,1],results[:,2])
    acc(1,results[:,1],results[:,2])
  ]

  x = linspace(0,2,100)
  y = []
  for i in x
    y = [y, acc(i,results[:,1],results[:,2])]
  end

  #plot(x,y, color="red", linewidth=2.0)
  writecsv("$filename.csv", results)

  return stats
end

function getstats(exps)
  cumstats = [0;0;0;0]

  namestart = searchindex(exps[1], "results")
  nameend = rsearchindex(exps[1], "result")

  figtitle = exps[1][namestart+8:nameend-2]
  figtype = exps[1][end-3:end]

  println("Stats for $figtitle  $figtype ")

  #figure()
  #title("$figtitle $figtype")
  for e in exps
    stats = processkeelrslt(e)
    cumstats += stats
    #println("file ",e," mse ",stats[1]," mad ",stats[2]," t05 ",stats[3]," t1 ",stats[4] )
  end

  cumstats = cumstats/5
  println("    mean stats: mse ",cumstats[1]," mad ",cumstats[2]," t05 ",cumstats[3]," t1 ",cumstats[4] )
end

# main

#dirs = ["exp", "exp8"]
dirs = ["expd","expd2","expz","expz2"]
for d in dirs
  expstst = findindir("/home/magnux/Desktop/experimentsnew/$d",".tst")
  expstra = findindir("/home/magnux/Desktop/experimentsnew/$d",".tra")

  getstats(expstst)
  getstats(expstra)
  println("***********************************************************************************************")
end


#close("all")
