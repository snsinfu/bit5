// error: Uncaught ReferenceError: process is not defined
// https://github.com/skypackjs/skypack-cdn/issues/127

import React from "https://cdn.skypack.dev/react@17.0.2?dts";
import ReactDOMServer from "https://cdn.skypack.dev/react-dom@17.0.2/server?dts";

interface AppProps {
  title: string;
  children: React.ReactNode;
}

function App(props: AppProps) {
  return (
    <div>
      <h1>{props.title}</h1>
      {props.children}
    </div>
  );
}

const app = <App title="SSR">Quick brown fox</App>;
console.log(ReactDOMServer.renderToString(app));
