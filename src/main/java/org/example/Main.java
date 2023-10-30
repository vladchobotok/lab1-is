package org.example;

import org.apache.jena.ontology.*;
import org.apache.jena.rdf.model.*;
import org.apache.jena.vocabulary.RDFS;
import org.apache.jena.vocabulary.XSD;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class Main {

    public static void main(String[] args) {
        Model model = ModelFactory.createDefaultModel();
        OntModel ontology = ModelFactory.createOntologyModel(OntModelSpec.OWL_DL_MEM, model);

        String baseNS = "https://onttest.net/";
        String ontNS = baseNS + "ontology#";

        ontology.setNsPrefix("", baseNS);
        ontology.setNsPrefix("ont", ontNS);

        OntClass userClass = ontology.createClass(ontNS + "User");
        OntClass ageGroupClass = ontology.createClass(ontNS + "AgeGroup");
        OntClass userGroupClass = ontology.createClass(ontNS + "UserGroup");

        ObjectProperty hasAgeGroup = ontology.createObjectProperty(ontNS + "hasAgeGroup");
        hasAgeGroup.addDomain(userClass);
        hasAgeGroup.addRange(ageGroupClass);

        ObjectProperty memberOf = ontology.createObjectProperty(ontNS + "memberOf");
        memberOf.addDomain(userClass);
        memberOf.addRange(userGroupClass);

        ObjectProperty canCommunicateWith = ontology.createObjectProperty(ontNS + "canCommunicateWith");
        canCommunicateWith.addDomain(userClass);
        canCommunicateWith.addRange(userClass);

        DatatypeProperty name = ontology.createDatatypeProperty(ontNS + "name");
        name.addDomain(userClass);
        name.addRange(RDFS.Literal);

        DatatypeProperty age = ontology.createDatatypeProperty(ontNS + "age");
        age.addDomain(userClass);
        age.addRange(XSD.xint);

        Individual firstUser = userClass.createIndividual(ontNS + "first_user");
        firstUser.addProperty(name, "Serhiy");
        firstUser.addLiteral(age, 14);

        Individual secondUser = userClass.createIndividual(ontNS + "second_user");
        secondUser.addProperty(name, "Olha");
        secondUser.addLiteral(age, 15);

        Individual thirdUser = userClass.createIndividual(ontNS + "third_user");
        thirdUser.addProperty(name, "Sophie");
        thirdUser.addLiteral(age, 28);

        Individual teenagers = ageGroupClass.createIndividual(ontNS + "teenagers");
        Individual adults = ageGroupClass.createIndividual(ontNS + "adults");

        Individual firstGroup = userGroupClass.createIndividual(ontNS + "first_group");
        Individual secondGroup = userGroupClass.createIndividual(ontNS + "second_group");

        firstUser.addProperty(hasAgeGroup, teenagers);
        secondUser.addProperty(hasAgeGroup, teenagers);
        thirdUser.addProperty(hasAgeGroup, adults);

        firstUser.addProperty(memberOf, firstGroup);
        secondUser.addProperty(memberOf, firstGroup);
        thirdUser.addProperty(memberOf, secondGroup);

        if (canCommunicate(firstUser, secondUser)) {
            firstUser.addProperty(canCommunicateWith, secondUser);
            secondUser.addProperty(canCommunicateWith, firstUser);
        }

        if (canCommunicate(firstUser, thirdUser)) {
            firstUser.addProperty(canCommunicateWith, thirdUser);
            thirdUser.addProperty(canCommunicateWith, firstUser);
        }

        if (canCommunicate(secondUser, thirdUser)) {
            secondUser.addProperty(canCommunicateWith, thirdUser);
            thirdUser.addProperty(canCommunicateWith, secondUser);
        }

        try (OutputStream outputStream = new FileOutputStream("ontology.owl")) {
            ontology.write(outputStream, "RDF/XML-ABBREV");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static boolean canCommunicate(Individual user1, Individual user2) {
        RDFNode ageGroup1 = user1.getPropertyValue(user1.getOntModel().getProperty(user1.getNameSpace() + "hasAgeGroup"));
        RDFNode ageGroup2 = user2.getPropertyValue(user2.getOntModel().getProperty(user2.getNameSpace() + "hasAgeGroup"));
        return ageGroup1.equals(ageGroup2);
    }
}
