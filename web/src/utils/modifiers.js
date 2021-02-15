import { defineAsyncComponent } from 'vue';

import Loader from '/@components/loader.vue';

export const defineComponent = (loader) => {
    console.log(loader)
    return defineAsyncComponent({
        loader,
        loadingComponent: Loader,
    });
}
