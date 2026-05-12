for conf in "$@"
do
    ./build/main ${conf}
done

for res in ./results/*
do
    python ./python/analyzer.py ${res}
done