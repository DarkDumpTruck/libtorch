echo "Compiling..."
g++ -O2 -o snake src/main.cpp || exit 1

echo "Testing..."
for i in {1..100}; do
    if ! [ -f testcases/$i.in ]; then
        break
    fi
    
    if ! ./snake < testcases/$i.in 2>/dev/null | diff - testcases/$i.out; then
        echo "$i.in failed"
    else
        echo "$i.in passed"
    fi
done