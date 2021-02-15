const path = require('path');
const srcPath = path.resolve(__dirname, './src');

const aliases = {
    '/@/': '',
    '/@pages/': 'pages/',
    '/@components': 'pages/components/',
    '/@utils/': 'utils/',
    '/@store/': 'store/',
    '/@mixins/': 'mixins/',
    '/@models/': 'models/',
}

module.exports = {
    // outDir: '../data/',
    rollupOutputOptions: {
        entryFileNames: `[name].js`,
        chunkFileNames: `[name].js`,
    },
    resolvers: [
        {
            requestToFile: publicPath => {
                return Object.entries(aliases).reduce((init, [ alias, filePath ]) => (
                    publicPath.startsWith(alias)
                        ? path.join(srcPath, publicPath.replace(alias, filePath))
                        : init
                ), undefined);
            },
            fileToRequest: filePath => {
                return Object.entries(aliases).reduce((init, [ alias, filePath ]) => (
                    filePath.startsWith(['/src', filePath].join('/'))
                        ? `${alias}${path.relative(srcPath, filePath)}`
                        : init
                ), undefined);
            }
        }
    ]
}
