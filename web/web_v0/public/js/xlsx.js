function downloadExcel(filename, type, data) {
  // Create excel workbook
  var wb = XLSX.utils.book_new();
  // Push in a new sheet
  wb.SheetNames.push(type);

  // Convert json data to sheet data
  var ws = XLSX.utils.json_to_sheet(data);

  // Add data to the sheet
  wb.Sheets[type] = ws;

  // Create download file
  var wbout = XLSX.write(wb, { bookType: "xlsx", type: "binary" });

  // Convert download file
  function s2ab(s) {
    var buf = new ArrayBuffer(s.length);
    var view = new Uint8Array(buf);
    for (var i = 0; i < s.length; i++) view[i] = s.charCodeAt(i) & 0xff;
    return buf;
  }

  // Download file
  saveAs(
    new Blob([s2ab(wbout)], { type: "application/octet-stream" }),
    `${filename.replaceAll(" ", "_").toLowerCase()}.xlsx`
  );
}
