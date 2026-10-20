import http from 'k6/http';
import { check, sleep } from 'k6';
export const options = { vus: 10, duration: '20s' };
export default function () {
  if (__ITER % 2 === 0) {
    const body = JSON.stringify({ title: `m${__ITER}`, assignee: 'k', priority: 3 });
    check(http.post('http://localhost:8080/tasks', body, {
      headers: { 'Content-Type': 'application/json' },
    }), { '201': (r) => r.status === 201 });
  } else {
    check(http.get('http://localhost:8080/health'), { '200': (r) => r.status === 200 });
  }
  sleep(0.4);
}
