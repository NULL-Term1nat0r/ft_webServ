// <!DOCTYPE html>
// <html>
// <head>
//     <title>CGI Example</title>
// </head>
// <body>
//
// <?php
// parse_str($_SERVER['QUERY_STRING'], $params);
// $name = $params['name'];
//
// echo "<h1>Hello dude, ";
// if ($name) {
//     echo "$name!";
// } else {
//     echo "No name provided.";
// }
// echo "</h1>";
// ?>
//
// </body>
// </html>
//

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Cyberpunk CGI Result</title>
    <style>
        body {
            background-color: #0f0f0f;
            color: #00ff00;
            font-family: 'Courier New', monospace;
            margin: 0;
            padding: 20px;
            box-sizing: border-box;
        }

        h1 {
            color: #29b6f6;
        }
    </style>
</head>
<body>

<?php
parse_str($_SERVER['QUERY_STRING'], $params);
$name = $params['name'];

echo "<h1>Hello dude, ";
if ($name) {
    echo "$name!";
} else {
    echo "No name provided.";
}
echo "</h1>";
?>

</body>
</html>
