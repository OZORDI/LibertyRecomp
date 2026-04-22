// Paste into VS Code DevTools Console (Help → Toggle Developer Tools → Console)
// Multi-pass scroll: goes UP first, then DOWN, then fills gaps. Downloads .txt.

(async function() {
  const chatList = document.querySelector('.interactive-list .monaco-list[role="list"]');
  if (!chatList) { console.error('No chat list found.'); return; }

  const scrollableEl = chatList.querySelector('.monaco-scrollable-element');
  const anyRow = document.querySelector('.interactive-list .monaco-list-row[aria-setsize]');
  const totalRows = anyRow ? parseInt(anyRow.getAttribute('aria-setsize')) : 82;
  console.log(`Chat has ${totalRows} rows.`);

  const collected = new Map();

  function collectVisible() {
    document.querySelectorAll(
      '.interactive-list .monaco-list-row.request, .interactive-list .monaco-list-row.response'
    ).forEach(row => {
      const idx = row.getAttribute('data-index');
      if (!idx || collected.has(idx)) return;
      const isUser = row.classList.contains('request');
      const role = isUser ? 'USER' : 'AGENT';
      let parts = [];
      if (isUser) {
        const v = row.querySelector('.value');
        if (v) {
          const md = v.querySelectorAll('.chat-markdown-part.rendered-markdown');
          md.length ? md.forEach(b => parts.push(b.innerText.trim()))
                    : parts.push(v.innerText.trim());
        }
      } else {
        const v = row.querySelector('.value');
        if (v) for (const c of v.children) {
          if (c.classList.contains('chat-markdown-part') && c.classList.contains('rendered-markdown')) {
            const t = c.innerText.trim(); if (t) parts.push(t);
          }
          if (c.classList.contains('chat-tool-invocation-part')) {
            const p = c.querySelector('.progress-container .rendered-markdown');
            if (p) { const t = p.innerText.trim(); if (t) parts.push('[tool] ' + t); }
            else {
              const cl = c.querySelector('.chat-confirmation-widget-title .rendered-markdown');
              if (cl) { const t = cl.innerText.trim(); if (t) parts.push('[tool] ' + t); }
            }
          }
        }
      }
      const text = parts.join('\n\n');
      if (text) collected.set(idx, { role, text });
    });
  }

  function wheel(dy) {
    for (const t of [chatList, scrollableEl].filter(Boolean)) {
      t.dispatchEvent(new WheelEvent('wheel', {
        deltaY: dy, deltaMode: 0, bubbles: true, cancelable: true
      }));
    }
  }

  function wait(ms) { return new Promise(r => setTimeout(r, ms)); }

  function getTop() {
    const r = chatList.querySelector('.monaco-list-rows');
    const m = (r?.style.top || '').match(/-?[\d.]+/);
    return m ? parseFloat(m[0]) : 0;
  }

  // PASS 1: Scroll ALL the way up (negative deltaY = scroll up)
  console.log('Pass 1: Scrolling to top...');
  for (let i = 0; i < 200; i++) {
    wheel(-3000);
    await wait(30);
  }
  await wait(500);
  collectVisible();
  console.log(`After scroll-up: top=${getTop()}, collected=${collected.size}`);

  // PASS 2: Scroll all the way down slowly, collecting
  console.log('Pass 2: Scrolling down...');
  let noMove = 0;
  for (let i = 0; i < 800; i++) {
    const before = getTop();
    wheel(400);
    await wait(150);
    collectVisible();
    const after = getTop();
    if (i % 20 === 0) console.log(`Down ${i}, top=${after}, collected=${collected.size}/${totalRows}`);
    if (Math.abs(after - before) < 1) { noMove++; if (noMove > 15) break; } else noMove = 0;
  }
  console.log(`After down pass: collected=${collected.size}`);

  // PASS 3: Scroll back up to catch anything missed
  console.log('Pass 3: Scrolling back up...');
  noMove = 0;
  for (let i = 0; i < 800; i++) {
    const before = getTop();
    wheel(-400);
    await wait(150);
    collectVisible();
    const after = getTop();
    if (i % 20 === 0) console.log(`Up ${i}, top=${after}, collected=${collected.size}/${totalRows}`);
    if (Math.abs(after - before) < 1) { noMove++; if (noMove > 15) break; } else noMove = 0;
  }
  console.log(`After up pass: collected=${collected.size}`);

  // Check for gaps
  const indices = [...collected.keys()].map(Number).sort((a, b) => a - b);
  const missing = [];
  for (let i = 0; i < totalRows; i++) {
    if (!collected.has(String(i))) missing.push(i);
  }
  if (missing.length) console.log(`Missing indices: ${missing.join(', ')}`);

  // Format and download
  const sorted = [...collected.entries()]
    .sort((a, b) => parseInt(a[0]) - parseInt(b[0]))
    .map(([, data]) => data);

  const sep = '='.repeat(60);
  let output = '';
  sorted.forEach((t, i) => {
    output += sep + '\n';
    output += `[${i + 1}] ${t.role}\n`;
    output += sep + '\n';
    output += t.text + '\n\n';
  });

  const blob = new Blob([output], { type: 'text/plain' });
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url;
  a.download = 'chat_export_' + new Date().toISOString().slice(0, 10) + '.txt';
  document.body.appendChild(a);
  a.click();
  document.body.removeChild(a);
  URL.revokeObjectURL(url);

  console.log(`\nDone! Exported ${sorted.length}/${totalRows} turns.`);
})();
