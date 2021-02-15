import { reactive, readonly } from 'vue';

const state = reactive({
    loading: 0,
    isImageLoaded: null,
    imageList: [],
    gallery: {
        links: [],
    },
    matrixParams: {
        effects: [],
        games: [],
        refreshingTime: 5000,
        colorsCorrections: [100,100,100],
        speedMax: 1000,
        height: 0,
        width: 0,
        speed: 20,
        brightness: 100
    },
    matrixContent: [],
    refreshingTime: 5000,
});

const setLoading = loading => loading ? state.loading++ : state.loading--;
const setImageLoaded = loaded => state.isImageLoaded = loaded;
const setImageList = imageList => state.imageList = imageList;

const setGalleryLinks = links => state.gallery.links = links;
const clearGalleryLinks = () => state.gallery.links = [];

const setMatrixParams = matrixParams => {
    state.matrixParams = matrixParams;
    state.refreshingTime = matrixParams.refreshingTime || 5000;
}
const setMatrixContent = content => state.matrixContent = content;
const setRefreshingTime = refreshingTime => state.refreshingTime = refreshingTime;
const setCorrections = corrections => state.matrixParams.colorsCorrections = corrections;

export default {
    state: readonly(state),

    setLoading,
    setImageList,
    setGalleryLinks,
    clearGalleryLinks,
    setImageLoaded,
    setMatrixParams,
    setMatrixContent,
    setRefreshingTime,
    setCorrections
};


