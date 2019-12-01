module.exports = function (options) {
    //Import the mock data json file
    const mockData = require('./MOCK_DATA.json');

    //Add the patterns and their corresponding functions
    this.add({role:'product',cmd:'getProductURL'}, productURL);
    this.add({role:'product',cmd:'getProductName'}, productName);


    //To DO: add the pattern functions and describe the logic inside the function
    function productURL(msg, respond) {
	var productFound = '';
	for (var i = 0; i < mockData.length; i++) {
	    if (mockData[i].product_id == msg.productId) {
		productFound = i + 1;
		break;
	    }
	}
	if (productFound) {
	    respond(null, {result: mockData[productFound-1].product_url});
	} 
	else {
	    result(null, {result: ''});
	}
    }
    function productName(msg, respond) {
	var productFound = '';
	for (var i = 0; i < mockData.length; i++) {
	    if (mockData[i].product_id == msg.productId) {
		productFound = i + 1;
		break;
	    }
	}
	if (productFound) {
	    respond(null, {result: mockData[productFound-1].product_name});
	} 
	else {
	    result(null, {result: ''});
	}
    }
}
