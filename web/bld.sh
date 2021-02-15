npm run build
gzip -r ./dist/_assets
rm -r ../data/_assets
rm ../data/index.html
cp -r ./dist/* ../data
cd ../data

if [[ ! -e ./gifs ]]; then
    mkdir ./gifs
elif [[ ! -d ./gifs ]]; then	
    echo "./gifs already exists but is not a directory" 1>&2
fi
cp -r ../web/gifs/* ./gifs

if [[ ! -e ./imgs ]]; then
    mkdir ./imgs
elif [[ ! -d ./imgs ]]; then	
    echo "./imgs already exists but is not a directory" 1>&2
fi
cp -r ../web/imgs/* ./imgs