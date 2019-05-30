// include script used: https://cdnjs.com/libraries/Chart.js
// include this in your html:  <canvas id="myChart" width="400" height="400"></canvas>
// CSS: <link rel="stylesheet" type="text/css" href="path/to/chartjs/dist/Chart.min.css">

// init Chart 
var myChart;

var i ;

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

function getSensorData() {
    var req = false;
    function dataReturned() {
        if (req.readyState == 4) {
            if (req.status == 200) {
                if (myChart.data.labels.length > 20) {
                    myChart.data.labels.shift();
                    myChart.data.datasets[0].data.shift();              // Delete first element
                }
                myChart.data.datasets[0].data.push(req.responseText) ;  // add response data in the chart array
                myChart.data.labels.push(i);
                myChart.update();
            }
        }
    }

    if (window.XMLHttpRequest) {
        req = new XMLHttpRequest();
    }
    else if (window.ActiveXObject) {
        req = new ActiveXObject("Microsoft.XMLHTTP");
    }
    if (req) {
        req.open("GET", "/get_data?id=" + Math.random(), true);
        req.onreadystatechange = dataReturned;
        req.send(null);
    }
}
function getrandomData() {
    var rand = Math.floor(Math.random() * 10);      // returns a random integer from 0 to 9
    // delete the first element if 20 elements are reached
    if (myChart.data.labels.length > 20) {
        myChart.data.labels.shift();
        myChart.data.datasets[0].data.shift();              // Delete first element
    }
    myChart.data.datasets[0].data.push(rand+10) ;
    myChart.data.labels.push(i);
    myChart.update();
}

async function getDataLoop() {
    while (true) {
         getSensorData();
//        getrandomData();
        i++;
        await sleep(1000);
    }
}

window.onload = function () {
    var ctx = document.getElementById('myChart');
    myChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [0, 1, 2, 3, 4, 5],
            datasets: [{
                label: 'Lux',
                data: [],
                backgroundColor: [
                    'rgba(255, 99, 132, 0.2)',
                    'rgba(54, 162, 235, 0.2)',
                    'rgba(255, 206, 86, 0.2)',
                    'rgba(75, 192, 192, 0.2)',
                    'rgba(153, 102, 255, 0.2)',
                    'rgba(255, 159, 64, 0.2)'
                ],
                borderColor: [
                    'rgba(255, 99, 132, 1)',
                    'rgba(54, 162, 235, 1)',
                    'rgba(255, 206, 86, 1)',
                    'rgba(75, 192, 192, 1)',
                    'rgba(153, 102, 255, 1)',
                    'rgba(255, 159, 64, 1)'
                ],
                borderWidth: 1
            }]
        },
        options: {
            scales: {
                yAxes: [{
                    ticks: {
                        beginAtZero: true
                    },
                    scaleLabel: {
                        display: true,
                        labelString: 'Lux'
                      }
                }],
                xAxes: [{
                    ticks: {
                        beginAtZero: true
                    },
                    scaleLabel: {
                        display: true,
                        labelString: 'Time in s'
                      }
                }],
                
            },
            legend: {
                display: true,
                labels: {
                    fontColor: 'rgb(255, 99, 132)'
                }
            }
        }
    });
    i = myChart.data.labels.length - 1;
    getDataLoop();
    
}


