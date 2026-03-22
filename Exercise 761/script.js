function solution(n) {
  let K = 0;
  let theta = Math.PI / n;
  const f = (k) => {return Math.sin(k*theta) - (k+n) * Math.tan(theta) * Math.cos(k*theta)};
  
  while (f(K) <0) {
    K += 1;
  }
  K -= 1;
  
  let a = (K*theta + Math.acos(2 * Math.sin(K*theta)/((K+n)*Math.tan(theta)) - Math.cos(K*theta)))/2;
  return (1/Math.cos(a)).toFixed(8);
}

$('#results').html(solution(6));