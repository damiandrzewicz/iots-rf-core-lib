(window.webpackJsonp=window.webpackJsonp||[]).push([[6],{373:function(e,t,a){},403:function(e,t,a){"use strict";a(373)},407:function(e,t,a){"use strict";a.r(t);a(84),a(39),a(7),a(116),a(117);var o={name:"CodeGroup",data:function(){return{codeTabs:[],activeCodeTabIndex:-1}},watch:{activeCodeTabIndex:function(e){this.activateCodeTab(e)}},mounted:function(){this.loadTabs()},methods:{changeCodeTab:function(e){this.activeCodeTabIndex=e},loadTabs:function(){var e=this;this.codeTabs=(this.$slots.default||[]).filter((function(e){return Boolean(e.componentOptions)})).map((function(t,a){return""===t.componentOptions.propsData.active&&(e.activeCodeTabIndex=a),{title:t.componentOptions.propsData.title,elm:t.elm}})),-1===this.activeCodeTabIndex&&this.codeTabs.length>0&&(this.activeCodeTabIndex=0),this.activateCodeTab(0)},activateCodeTab:function(e){this.codeTabs.forEach((function(e){e.elm&&e.elm.classList.remove("theme-code-block__active")})),this.codeTabs[e].elm&&this.codeTabs[e].elm.classList.add("theme-code-block__active")}}},n=(a(403),a(56)),c=Object(n.a)(o,(function(){var e=this,t=e.$createElement,a=e._self._c||t;return a("ClientOnly",[a("div",{staticClass:"theme-code-group"},[a("div",{staticClass:"theme-code-group__nav"},[a("ul",{staticClass:"theme-code-group__ul"},e._l(e.codeTabs,(function(t,o){return a("li",{key:t.title,staticClass:"theme-code-group__li"},[a("button",{staticClass:"theme-code-group__nav-tab",class:{"theme-code-group__nav-tab-active":o===e.activeCodeTabIndex},on:{click:function(t){return e.changeCodeTab(o)}}},[e._v("\n            "+e._s(t.title)+"\n          ")])])})),0)]),e._v(" "),e._t("default"),e._v(" "),e.codeTabs.length<1?a("pre",{staticClass:"pre-blank"},[e._v("// Make sure to add code blocks to your code group")]):e._e()],2)])}),[],!1,null,"deefee04",null);t.default=c.exports}}]);