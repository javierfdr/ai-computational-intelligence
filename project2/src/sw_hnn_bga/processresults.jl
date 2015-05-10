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

findindir(dir,regex) = filter(x->ismatch(regex,x), walkdir(dir))

function readhnnrslt(filename)
  filestream = open(filename, "r")
  readuntil(filestream, "LMode: ")
  mode = readline(filestream)[1:7]

  if mode == "CLASSIF"
    readuntil(filestream, "TRAINING")
    readuntil(filestream, "---> Mean Accuracy ")
    trainrslt = float64((readuntil(filestream, "%"))[1:end-1])

    readuntil(filestream, "VALIDATION")
    readuntil(filestream, "---> Mean Accuracy ")
    validrslt = float64(readuntil(filestream, "%")[1:end-1])

    readuntil(filestream, "TEST")
    readuntil(filestream, "---> Mean Accuracy ")
    testrslt = float64(readuntil(filestream, "%")[1:end-1])
  elseif mode == "REGRESS"
    readuntil(filestream, "TRAINING")
    readuntil(filestream, "(Mean) ")
    trainrslt = float64(readuntil(filestream, " (")[1:end-2])

    readuntil(filestream, "VALIDATION")
    readuntil(filestream, "(Mean) ")
    validrslt = float64(readuntil(filestream, " (")[1:end-2])

    readuntil(filestream, "TEST")
    readuntil(filestream, "(Mean) ")
    testrslt = float64(readuntil(filestream, " (")[1:end-2])
  else
    error("Error reading file: Unknown Mode in file")
  end

  return [trainrslt;validrslt;testrslt]
end


#datasets = ["Audiology","BostonHousing","CreditCard","Servo"]
datasets = ["Audiology","CreditCard","HorseColic","Servo"]
#resultpaths = ["Resultats","Resultats2","Resultats3"]
resultpaths = ["Exploit","Explore"]

ntype = ["h","n","r"]
ntypelong = ["hnn","mlp","rbf"]
path = "/home/magnux/Desktop/MAI/2nd Semester/ATCI/atci-projects/project2/src/sw_hnn_bga/Datasets"
outputpath = "/home/magnux/Desktop/MAI/2nd Semester/ATCI/atci-projects/project2/src/sw_hnn_bga/Results"
for r in resultpaths
  println("Result Set: $r")
  for s in datasets
    println("Data Set: $s")
    mkpath("$outputpath/$r/$s")
    for ttuple in zip(ntype,ntypelong)
      t, tl = ttuple
      println("Network Type: $t")
      files = findindir("$path/$s/$r", Regex(string("$t","_(SIGM|ID).info\$")))
      for f in files
        results = readhnnrslt(f)
        m = match(r"/([^/]*)_(SIGM|ID).info$",f)
        outname = (m.captures[1])[7:end-1]
        writecsv("$outputpath/$r/$s/$tl-$outname.csv", results)
      end
    end
  end
  println("***********************************************************************************************")
end
