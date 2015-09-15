<?php
    $tweet=$_GET["tweet"];
    
        session_start();
        require_once("twitteroauth.php"); //Path to twitteroauth library
        
        $twitteruser = "???"; //Twitter account to get tweets from
        $notweets = 1; //Number of tweets you want to receive
        
        //Get the following 4 lines from you Twitter API token info
        $consumerkey = "???";
        $consumersecret = "???";
        $accesstoken = "???";
        $accesstokensecret = "???";
        
        //Connect to Twitter API
        $connection = getConnectionWithAccessToken($consumerkey, $consumersecret, $accesstoken, $accesstokensecret);
        $tweets = $connection->get("https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=".$twitteruser."&count=".$notweets);
    
        //Prepare Tweets for Arduino
        $file='tweet.txt';
        file_put_contents($file, "");
        $result="!---!";
        foreach ($tweets as $status)
        {
    
            $result= $result."\n".$status->text."<br></br>";
            
        }
        file_put_contents($file, $result);
        
        echo "<!DOCTYPE html><html><head></head><body>$result</body></html>";
        
        function getConnectionWithAccessToken($cons_key, $cons_secret, $oauth_token, $oauth_token_secret) {
            $connection = new TwitterOAuth($cons_key, $cons_secret, $oauth_token, $oauth_token_secret);
            return $connection;
        }
    
?>