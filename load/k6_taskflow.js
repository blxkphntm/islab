import http from 'k6/http';
import { check, sleep } from 'k6';

export const options = {
  vus: 8,
  duration: '35s',
  thresholds: {
    http_req_failed: ['rate<0.01'],
    http_req_duration: ['p(95)<400'],
  },
};

export default function () {
  const payload = JSON.stringify({
    title: `k-${__VU}-${__ITER}`,
    assignee: 'kyznetsov',
    priority: 2,
    comment: 'load',
  });
  check(http.post('http://localhost:8080/tasks', payload, {
    headers: { 'Content-Type': 'application/json' },
  }), { '201': (r) => r.status === 201 });
  check(http.get('http://localhost:8080/tasks'), { '200': (r) => r.status === 200 });
  sleep(0.9);
}
